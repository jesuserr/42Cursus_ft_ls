/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:33 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/31 13:55:58 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Traverses the linked list of string filenames parsed from command line to
// create a new linked list of t_entry_data structures containing file needed
// metadata (stat info and name). If long listing is requested, it calculates
// the maximum widths for each field in the long listing format. Finally, it
// calls list_files() to print the files in the specified format and cleans up
// the allocated memory for the entries list and widths if applicable.
static void	files_listing(t_args *args)
{
	t_list			*list;
	t_widths		*widths;
	t_list			*entries_list;
	t_entry_data	*entry_data;
	char			*filename;

	list = args->cli_files_list;
	entries_list = NULL;
	while (list)
	{
		entry_data = malloc(sizeof(t_entry_data));
		filename = (char *)list->content;
		if (lstat(filename, &entry_data->stat_buf) != 0)
		{
			free(entry_data);
			list = list->next;
			continue ;
		}
		ft_strlcpy(entry_data->entry.d_name, filename, sizeof(entry_data->entry.d_name));
		ft_lstadd_back(&entries_list, ft_lstnew(entry_data));
		list = list->next;
	}
	widths = NULL;
	if (args->long_listing)
		widths = calculate_fields_widths(args, entries_list);
	list_files(args, entries_list, widths);
	if (args->long_listing && widths)
		free(widths);
	ft_lstclear(&entries_list, free);
}

// Traverses the linked list of string directory names parsed from command line,
// calling list_dirs() to process each directory.
static void	dirs_listing(t_args *args)
{
	t_list	*list;

	list = args->cli_dirs_list;
	while (list)
	{
		list_dirs(args, (char *)list->content);
		list = list->next;
	}
}

// Flushes printf buffer and frees memory allocated during program execution.
void	free_allocated_memory(t_args *args)
{
	ft_printf_flush();
	if (args->cli_files_list)
		ft_lstclear(&args->cli_files_list, free);
	if (args->cli_dirs_list)
		ft_lstclear(&args->cli_dirs_list, free);
	return ;
}

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              MAIN FUNCTION
*/
int	main(int argc, char **argv)
{
	t_args	args;

	(void)argc;
	ft_bzero(&args, sizeof(t_args));
	args.id_cache.last_uid = -1;
	args.id_cache.last_gid = -1;
	parse_arguments(argv, &args);
	files_listing(&args);
	dirs_listing(&args);
	free_allocated_memory(&args);
	return (args.exit_status);
}
