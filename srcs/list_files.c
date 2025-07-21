/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:20:48 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/21 09:12:45 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Build full path for a file or directory entry. Used for lstat and recursive
// calls to list_files. It allocates enough memory for the full path, including
// the null terminator. It also ensures that there is a '/' between the current
// path and the entry name, unless the current path ends with '/'.
static char	*build_full_path(const char *current_path, const struct dirent *entry)
{
	char		*full_path;
	uint64_t	path_len;
	uint64_t	entry_d_name_len;

	path_len = ft_strlen(current_path);
	entry_d_name_len = ft_strlen(entry->d_name);
	full_path = malloc(path_len + entry_d_name_len + 2);
	ft_strlcpy(full_path, current_path, path_len + entry_d_name_len + 2);
	if (path_len > 0 && current_path[path_len - 1] != '/')
		ft_strlcat(full_path, "/", path_len + entry_d_name_len + 2);
	ft_strlcat(full_path, entry->d_name, path_len + entry_d_name_len + 2);
	return (full_path);
}

// Copies the contents of a dirent struct to another dirent struct.
// A simple ft_memcpy would not work since struct dirent is a variable-length
// structure where d_name is the last field and its actual size depends on the
// filename length. Using sizeof(struct dirent) copies beyond the allocated
// memory for that specific entry, causing invalid reads.
// Doing ft_memcpy(&new_entry_data->entry, entry, sizeof(struct dirent)) was
// making valgrind complain.
static void	copy_dirent_struct(struct dirent *dest, const struct dirent *src)
{
	dest->d_ino = src->d_ino;
	dest->d_off = src->d_off;
	dest->d_reclen = src->d_reclen;
	dest->d_type = src->d_type;
	ft_strlcpy(dest->d_name, src->d_name, sizeof(dest->d_name));
}

// Processes subdirectories for recursive listing. Sorts the subdirectory list
// by name or time, then recursively calls list_files() on each subdirectory
// using their full paths. Properly manages memory by freeing paths and clearing
// the subdirectories list after processing.
static void	process_subdirs(t_args *args, t_list *subdirs_list, const char *current_path)
{
	t_list			*current_subdir;
	char			*subdir_full_path;
	struct dirent	*subdir_entry;

	if (args->sort_by_time)
		sort_list(&subdirs_list, compare_stat_times, args->reverse);
	else
		sort_list(&subdirs_list, compare_d_names, args->reverse);
	current_subdir = subdirs_list;
	while (current_subdir)
	{
		subdir_entry = &((t_entry_data *)current_subdir->content)->entry;
		subdir_full_path = build_full_path(current_path, subdir_entry);
		list_files(args, subdir_full_path);
		free(subdir_full_path);
		current_subdir = current_subdir->next;
	}
	ft_lstclear(&subdirs_list, free);
}

// Recursively lists files and directories in the specified path. Opens the
// directory, reads each entry, and stores them in a linked list along with
// their stat information. Entries are sorted by name or modification time based
// on arguments. After printing the current directory contents, it processes
// subdirectories recursively if the recursive flag is set. Memory is properly
// cleaned up after processing.
void	list_files(t_args *args, const char *current_path)
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
		return ;
	}
	entry = readdir(directory);
	while (entry)
	{
		if (entry->d_name[0] == '.' && !args->all)
		{
			entry = readdir(directory);
			continue ;
		}
		new_entry_data = malloc(sizeof(t_entry_data));
		full_path = build_full_path(current_path, entry);
		lstat(full_path, &new_entry_data->stat_buf);
		copy_dirent_struct(&new_entry_data->entry, entry);
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
	if (args->sort_by_time)
		sort_list(&entries_list, compare_stat_times, args->reverse);
	else
		sort_list(&entries_list, compare_d_names, args->reverse);
	print_list(args, entries_list, current_path);
	if (subdirs_list)
		process_subdirs(args, subdirs_list, current_path);
	closedir(directory);
	ft_lstclear(&entries_list, free);
}
