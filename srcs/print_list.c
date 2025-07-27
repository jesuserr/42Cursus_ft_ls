/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:24:34 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/27 14:52:36 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Build full path for a file or directory entry. Used for lstat and recursive
// calls to list_dirs(). It allocates enough memory for the full path, including
// the null terminator. It also ensures that there is a '/' between the current
// path and the entry name, unless the current path ends with '/'.
char	*build_full_path(const char *current_path, const struct dirent *entry)
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

// Prints file name with proper formatting (long or simple). Handles filenames
// containing spaces by wrapping them in single quotes as real ls. For symbolic
// links, displays the link target using readlink() and shows the "-> target"
// format. If target path contains spaces, it's also quoted. Handles readlink
// errors by displaying "?" for unreadable link targets.
void	print_file_name(t_args *args, t_entry_data *entry_data, \
const char *current_path)
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
	if (!args->long_listing && !args->one_file_per_line)
		ft_printf("  ");
	if (S_ISLNK(entry_data->stat_buf.st_mode) && args->long_listing)
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
// Handles directory headers when multiple directories are listed, recursion is
// enabled or an error occurred. Delegates to print_long_format for detailed
// listings (-l option) or iterates through entries calling print_file_name for
// simple format.
void	print_list(t_args *args, t_list *entries_list, const char *current_path)
{
	if (!args->first_printing)
		ft_printf("\n");
	else
		args->first_printing = false;
	if (ft_lstsize(args->cli_dirs_list) > 1 || args->recursive || \
	(args->cli_dirs_list && args->cli_files_list) || args->exit_status != 0)
	{
		if (ft_strchr(current_path, ' ') != NULL)
			ft_printf("'%s':\n", current_path);
		else
			ft_printf("%s:\n", current_path);
	}
	if (args->long_listing)
		print_long_format(args, entries_list, current_path);
	else
	{
		while (entries_list)
		{
			print_file_name(args, (t_entry_data *)entries_list->content, current_path);
			entries_list = entries_list->next;
			if (args->one_file_per_line && entries_list)
				ft_printf("\n");
		}
		ft_printf("\n");
	}
}
