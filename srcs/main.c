/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:33 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/18 14:07:49 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_listing(t_args *args, t_list *entries_list, const char *current_path)
{
	t_entry_data	*entry_data;

	ft_printf("%s:\n", current_path);
	while (entries_list)
	{
		entry_data = (t_entry_data *)entries_list->content;
		if (entry_data->entry.d_name[0] == '.' && !args->all)
		{
			entries_list = entries_list->next;
			continue ;
		}
		ft_printf("%s  ", entry_data->entry.d_name);
		entries_list = entries_list->next;
	}
	ft_printf("\n\n");
}

// Frees the memory allocated during program execution.
void	free_allocated_memory(t_args *args)
{
	if (args->cli_files_list)
		ft_lstclear(&args->cli_files_list, free);
	return ;
}

// exiting if closedir fails is an issue if the function is recursive since
// the function should return -1 if it fails, not exit the program.
void	list_files(t_args *args, const char *current_path)
{
	DIR				*directory;
	struct dirent	*entry;
	t_list			*entries_list;
	t_entry_data	*new_entry_data;
	char			*full_path;

	entries_list = NULL;
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
		new_entry_data = malloc(sizeof(t_entry_data));
		full_path = build_full_path(current_path, entry);
		lstat(full_path, &new_entry_data->stat_buf);
		copy_dirent_struct(&new_entry_data->entry, entry);
		ft_lstadd_back(&entries_list, ft_lstnew(new_entry_data));
		if (entry->d_type == DT_DIR && ft_strcmp(entry->d_name, ".") != 0 && \
		ft_strcmp(entry->d_name, "..") != 0 && args->recursive)
			list_files(args, full_path);
		free(full_path);
		entry = readdir(directory);
	}
	print_listing(args, entries_list, current_path);
	closedir(directory);
	ft_lstclear(&entries_list, free);
}

int	main(int argc, char **argv)
{
	t_args		args;
	t_list		*files_list;

	(void)argc;
	ft_bzero(&args, sizeof(t_args));
	parse_arguments(argv, &args);
	files_list = args.cli_files_list;
	while (files_list)
	{
		list_files(&args, (char *)files_list->content);
		files_list = files_list->next;
	}
	free_allocated_memory(&args);
	return (EXIT_SUCCESS);
}
