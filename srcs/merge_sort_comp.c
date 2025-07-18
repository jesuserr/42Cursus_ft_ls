/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort_comp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:17:13 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/18 21:17:14 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Comparison function to sort the list of command line arguments by their name.
// List is contained in args->cli_files_list. In that case *content of the list
// nodes is just a string (file/directory name).
int	compare_names_cli(const void *a, const void *b, bool reverse)
{
	if (reverse)
		return (ft_strcmp((char *)b, (char *)a));
	else
		return (ft_strcmp((char *)a, (char *)b));
}

// Comparison function to sort the list of directory entries by their d_name.
int	compare_d_names(const void *a, const void *b, bool reverse)
{
	t_entry_data	*entry_a;
	t_entry_data	*entry_b;

	entry_a = (t_entry_data *)a;
	entry_b = (t_entry_data *)b;
	if (reverse)
		return (ft_strcmp(entry_b->entry.d_name, entry_a->entry.d_name));
	else
		return (ft_strcmp(entry_a->entry.d_name, entry_b->entry.d_name));
}

// Comparison function to sort the list of directory entries by their st_mtime.
// Time of last modification is used to sort the entries.
// First approach was to subtract the two times and return the result, but
// that approach was not safe because the result could overflow if difference
// between both time was bigger than an int.
// When last modification times are the same (same second), the comparison is
// done by comparing the nanoseconds (st_mtim.tv_nsec) to ensure a stable sort.
// As last resort, if both times and nanoseconds are the same, the names
// are compared to ensure a stable sort.
int	compare_stat_times(const void *a, const void *b, bool reverse)
{
	t_entry_data	*entry_a;
	t_entry_data	*entry_b;
	int8_t			result;

	entry_a = (t_entry_data *)a;
	entry_b = (t_entry_data *)b;
	result = -1;
	if (reverse)
		result = 1;
	if (entry_a->stat_buf.st_mtime > entry_b->stat_buf.st_mtime)
		return (1 * result);
	if (entry_a->stat_buf.st_mtime < entry_b->stat_buf.st_mtime)
		return (-1 * result);
	if (entry_a->stat_buf.st_mtim.tv_nsec > entry_b->stat_buf.st_mtim.tv_nsec)
		return (1 * result);
	if (entry_a->stat_buf.st_mtim.tv_nsec < entry_b->stat_buf.st_mtim.tv_nsec)
		return (-1 * result);
	return (ft_strcmp(entry_b->entry.d_name, entry_a->entry.d_name));
}
