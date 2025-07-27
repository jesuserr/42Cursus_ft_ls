/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_routines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:20:48 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/27 14:56:28 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Traverses the linked list of files and prints each file entry in the
// specified format (long or simple). Reminder, used for command-line file
// arguments, not directory contents.
void	list_files(t_args *args, t_list *entries_list, t_widths *widths)
{
	t_list			*list;
	t_entry_data	*entry_data;
	char			*current_path;

	list = entries_list;
	while (list)
	{
		entry_data = (t_entry_data *)list->content;
		current_path = entry_data->entry.d_name;
		if (args->long_listing)
			print_long_line(args, entry_data, current_path, widths);
		else
			print_file_name(args, entry_data, current_path);
		args->first_printing = false;
		list = list->next;
		if (!list && !args->long_listing)
			ft_printf("\n");
	}
}

// Determines if file/directory entry should be skipped based on flags:
// -a shows all, -A shows all except "." and "..", default hides dotfiles.
static bool	skip_entry(t_args *args, struct dirent *entry)
{
	if (args->all)
		return (false);
	else if (args->almost_all)
	{
		if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0)
			return (true);
		return (false);
	}
	if (entry->d_name[0] == '.')
		return (true);
	return (false);
}

// Applies the appropriate sorting algorithm based on the user's options.
// entries_list passed as a double pointer to allow modification of the list
// in place.
static void	apply_sort_algorithm(t_args *args, t_list **entries_list)
{
	if (args->sort_by_time)
		sort_list(entries_list, compare_stat_times, args->reverse);
	else if (args->sort_by_size)
		sort_list(entries_list, compare_stat_sizes, args->reverse);
	else
		sort_list(entries_list, compare_d_names, args->reverse);
}

// Processes subdirectories for recursive listing. Sorts the subdirectory list
// by name, time or size and then recursively calls list_dirs() on each
// subdirectory using their full paths. Properly manages memory by freeing paths
// and clearing the subdirectories list after processing.
static void	process_subdirs(t_args *args, t_list *subdirs_list, \
const char *current_path)
{
	t_list			*current_subdir;
	char			*subdir_full_path;
	struct dirent	*subdir_entry;

	apply_sort_algorithm(args, &subdirs_list);
	current_subdir = subdirs_list;
	while (current_subdir)
	{
		subdir_entry = &((t_entry_data *)current_subdir->content)->entry;
		subdir_full_path = build_full_path(current_path, subdir_entry);
		list_dirs(args, subdir_full_path);
		free(subdir_full_path);
		current_subdir = current_subdir->next;
	}
	ft_lstclear(&subdirs_list, free);
}

// Recursively lists files and directories in the specified path. Opens the
// directory, reads each entry, and stores them in a linked list along with
// their stat information. Entries are sorted by name, modification time or size
// based on options. After printing the current directory contents, it processes
// subdirectories recursively if the recursive flag is set. Memory is properly
// cleaned up after processing.
void	list_dirs(t_args *args, const char *current_path)
{
	DIR				*directory;
	struct dirent	*entry;
	t_list			*entries_list, *subdirs_list;
	t_entry_data	*new_entry_data, *new_subdir_data;
	char			*full_path;

	entries_list = NULL;
	subdirs_list = NULL;
	directory = opendir(current_path);
	if (directory == NULL)
	{
		ft_printf("ft_ls: cannot open directory '%s': ", current_path);
		perror("");
		if (args->exit_status != EXIT_SERIOUS_ERROR)
			args->exit_status = EXIT_MINOR_ERROR;
		return ;
	}
	entry = readdir(directory);
	while (entry)
	{
		if (skip_entry(args, entry))
		{
			entry = readdir(directory);
			continue ;
		}
		new_entry_data = malloc(sizeof(t_entry_data));
		full_path = build_full_path(current_path, entry);
		lstat(full_path, &new_entry_data->stat_buf);
		ft_strlcpy(new_entry_data->entry.d_name, entry->d_name, \
		sizeof(new_entry_data->entry.d_name));
		ft_lstadd_back(&entries_list, ft_lstnew(new_entry_data));
		if (entry->d_type == DT_DIR && ft_strcmp(entry->d_name, ".") != 0 && \
		ft_strcmp(entry->d_name, "..") != 0 && args->recursive)
		{
			new_subdir_data = malloc(sizeof(t_entry_data));
			ft_memcpy(new_subdir_data, new_entry_data, sizeof(t_entry_data));
			ft_lstadd_back(&subdirs_list, ft_lstnew(new_subdir_data));
		}
		free(full_path);
		entry = readdir(directory);
	}
	apply_sort_algorithm(args, &entries_list);
	print_list(args, entries_list, current_path);
	if (subdirs_list)
		process_subdirs(args, subdirs_list, current_path);
	closedir(directory);
	ft_lstclear(&entries_list, free);
}
