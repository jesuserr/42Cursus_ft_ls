/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:24:34 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/13 17:19:55 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Returns appropriate ANSI color code based on file type and permissions.
// Handles directories, special files, symbolic links, setuid/setgid,
// executables and devices.
static const char	*get_entity_color(t_entry_data *entry_data)
{
	mode_t	mode;

	mode = entry_data->stat_buf.st_mode;
	if (S_ISDIR(mode))
	{
		if (mode & S_ISVTX)
			return (COLOR_STICKY_DIR);
		return (COLOR_DIRECTORY);
	}
	if (S_ISSOCK(mode))
		return (COLOR_SOCKET);
	if (S_ISFIFO(mode))
		return (COLOR_NAMED_PIPE);
	if (S_ISLNK(mode))
		return (COLOR_SYMLINK);
	if (mode & S_ISUID)
		return (COLOR_SETUID_FILE);
	if (mode & S_ISGID)
		return (COLOR_SETGID_FILE);
	if (mode & S_IXUSR || mode & S_IXGRP || mode & S_IXOTH)
		return (COLOR_EXECUTABLE);
	if (S_ISBLK(mode) || S_ISCHR(mode))
		return (COLOR_NAMED_PIPE);
	return (COLOR_RESET);
}

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
	char		*file_name;
	char		*full_path;
	char		buffer[PATH_MAX];
	int64_t		read_bytes;
	const char	*color;

	file_name = entry_data->entry.d_name;
	color = COLOR_RESET;
	if (!args->no_colors)
		color = get_entity_color(entry_data);
	if (args->quoting && ft_strchr(file_name, ' ') != NULL)
		ft_printf("%s'%s'%s", color, file_name, COLOR_RESET);
	else
		ft_printf("%s%s%s", color, file_name, COLOR_RESET);
	if (S_ISLNK(entry_data->stat_buf.st_mode) && args->long_listing)
	{
		ft_printf(" -> ");
		full_path = build_full_path(current_path, &entry_data->entry);
		read_bytes = readlink(full_path, buffer, sizeof(buffer) - 1);
		if (read_bytes == -1)
			read_bytes = readlink(file_name, buffer, sizeof(buffer) - 1);
		if (read_bytes != -1)
		{
			buffer[read_bytes] = '\0';
			if (args->quoting && ft_strchr(buffer, ' ') != NULL)
				ft_printf("'%s'", buffer);
			else
				ft_printf("%s", buffer);
		}
		else
			ft_printf("?");
		free(full_path);
	}
}

// Prints directory entries in a multi-column format based on terminal width.
// Calculates optimal column layout using terminal size, longest filename and
// number of entries. In case that terminal is smaller than the longest
// filename (columns = 0, divide-by-zero!!) it will print only one column.
// Using list traversal, it prints each file name with proper spacing and
// alignment. Very proud of this function, it took some time to get it right,
// specially the traversal logic through the linked list.
static void	print_in_columns(t_args *args, t_list *entries_list)
{
	struct winsize	ws;
	t_entry_data	*entry_data;
	t_list			*list;
	uint16_t		longest_name, columns, rows, entries;

	longest_name = 0;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0)
		ws.ws_col = 80;
	entries = ft_lstsize(entries_list);
	list = entries_list;
	while (list)
	{
		entry_data = (t_entry_data *)list->content;
		if (ft_strlen(entry_data->entry.d_name) > longest_name)
			longest_name = ft_strlen(entry_data->entry.d_name);
		list = list->next;
	}
	longest_name += 2;
	columns = ws.ws_col / longest_name;
	if (columns == 0)
		columns = 1;
	rows = entries / columns;
	if (entries % columns)
		rows++;
	list = entries_list;
	for (uint32_t j = 0; j < rows && list; j++)
	{
		while (list)
		{
			entry_data = (t_entry_data *)list->content;
			print_file_name(args, entry_data, NULL);
			print_blanks(longest_name - ft_strlen(entry_data->entry.d_name));
			for (uint32_t i = 0; i < rows && list; i++)
				list = list->next;
		}
		list = entries_list;
		for (uint32_t i = 0; i < j + 1 && list; i++)
			list = list->next;
		ft_printf("\n");
	}
}

// Prints the complete listing of directory entries with appropriate formatting.
// Handles directory headers when multiple directories are listed, recursion is
// enabled or an error occurred. Delegates to print_long_format for detailed
// listings (-l option) or iterates through entries calling print_file_name for
// simple format.
void	print_list(t_args *args, t_list *entries_list, const char *current_path)
{
	t_list	*list;

	list = entries_list;
	if (!args->first_printing)
		ft_printf("\n");
	else
		args->first_printing = false;
	if (ft_lstsize(args->cli_dirs_list) > 1 || args->recursive || \
	(args->cli_dirs_list && args->cli_files_list) || args->exit_status != 0)
	{
		if (args->quoting && ft_strchr(current_path, ' ') != NULL)
			ft_printf("'%s':\n", current_path);
		else
			ft_printf("%s:\n", current_path);
	}
	if (args->long_listing)
		print_long_format(args, entries_list, current_path);
	else if (args->one_file_per_line)
	{
		while (entries_list)
		{
			print_file_name(args, (t_entry_data *)entries_list->content, current_path);
			ft_printf("\n");
			entries_list = entries_list->next;
		}
	}
	else
		print_in_columns(args, entries_list);
	if (args->total_files)
		ft_printf("%d file(s)\n", ft_lstsize(list));
}
