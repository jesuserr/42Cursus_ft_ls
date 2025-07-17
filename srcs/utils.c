/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:03:03 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/17 23:42:14 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Build full path for a file or directory entry. Used for lstat and recursive
// calls to list_files. It allocates enough memory for the full path, including
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

// Copies the contents of a dirent struct to another dirent struct.
// A simple ft_memcpy would not work since struct dirent is a variable-length
// structure where d_name is the last field and its actual size depends on the
// filename length. Using sizeof(struct dirent) copies beyond the allocated
// memory for that specific entry, causing invalid reads.
// Doing ft_memcpy(&new_entry_data->entry, entry, sizeof(struct dirent)) was
// making valgrind complains.
void	copy_dirent_struct(struct dirent *dest, const struct dirent *src)
{
	dest->d_ino = src->d_ino;
	dest->d_off = src->d_off;
	dest->d_reclen = src->d_reclen;
	dest->d_type = src->d_type;
	ft_strlcpy(dest->d_name, src->d_name, sizeof(dest->d_name));
}

// Auxiliar function for debugging purposes. Delete at the end of the project.
void	print_list(t_list *list)
{
	while (list)
	{
		ft_printf("Content: %s\n", (char *)list->content);
		list = list->next;
	}
}
