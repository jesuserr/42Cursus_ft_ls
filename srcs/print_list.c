/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:24:34 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/24 00:03:43 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Prints file name with proper formatting (long or simple). Handles filenames
// containing spaces by wrapping them in single quotes as real ls. For symbolic
// links, displays the link target using readlink() and shows the "-> target"
// format. If target path contains spaces, it's also quoted. Handles readlink
// errors by displaying "?" for unreadable link targets.
void	print_file_name(t_entry_data *entry_data, const char *current_path, \
bool long_format)
{
	char	*file_name;
	char	*full_path;
	char	buffer[PATH_MAX];
	int64_t	read_bytes;

	file_name = entry_data->entry.d_name;
	if (ft_strchr(file_name, ' ') != NULL)
		ft_printf("'%s'", file_name);
	else
		ft_printf("%s", file_name);
	if (!long_format)
		ft_printf("  ");
	if (S_ISLNK(entry_data->stat_buf.st_mode) && long_format)
	{
		ft_printf(" -> ");
		full_path = build_full_path(current_path, &entry_data->entry);
		read_bytes = readlink(full_path, buffer, sizeof(buffer) - 1);
		if (read_bytes != -1)
		{
			buffer[read_bytes] = '\0';
			if (ft_strchr(buffer, ' ') != NULL)
				ft_printf("'%s'", buffer);
			else
				ft_printf("%s", buffer);
		}
		else
			ft_printf("?");
		free(full_path);
	}
}

// Prints the complete listing of directory entries with appropriate formatting.
// Handles directory headers when multiple directories are listed or recursion
// is enabled. Wraps directory paths containing spaces in quotes. Delegates to
// print_long_format for detailed listings (-l option) or iterates through
// entries calling print_file_name for simple format, ending with newline.
void	print_list(t_args *args, t_list *entries_list, const char *current_path)
{
	if (!args->first_printing)
		ft_printf("\n");
	else
		args->first_printing = false;
	if (ft_lstsize(args->cli_dirs_list) > 1 || args->recursive || \
	(args->cli_dirs_list && args->cli_files_list))
	{
		if (ft_strchr(current_path, ' ') != NULL)
			ft_printf("'%s':\n", current_path);
		else
			ft_printf("%s:\n", current_path);
	}
	if (args->long_listing)
		print_long_format(entries_list, current_path);
	else
	{
		while (entries_list)
		{
			print_file_name((t_entry_data *)entries_list->content, current_path, \
			args->long_listing);
			entries_list = entries_list->next;
		}
		ft_printf("\n");
	}
}
