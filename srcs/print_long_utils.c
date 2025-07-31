/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_long_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:38:38 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/31 12:12:16 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Last user and group IDs are cached to avoid repeated calls to getpwuid()
// and getgrgid(), which are very expensive operations. If the cached ID matches
// the current entry's ID, the cached name is used; otherwise, a new query is
// performed and the cache is updated.
void	check_cached_user_name(t_args *args, t_entry_data *entry_data)
{
	struct passwd	*user_info;

	if (args->id_cache.last_uid != entry_data->stat_buf.st_uid)
	{
		user_info = getpwuid(entry_data->stat_buf.st_uid);
		args->id_cache.last_uid = entry_data->stat_buf.st_uid;
		if (user_info)
			args->id_cache.last_user_name = user_info->pw_name;
		else
			args->id_cache.last_user_name = NULL;
	}
}

// Last user and group IDs are cached to avoid repeated calls to getpwuid()
// and getgrgid(), which are very expensive operations. If the cached ID matches
// the current entry's ID, the cached name is used; otherwise, a new query is
// performed and the cache is updated.
void	check_cached_group_name(t_args *args, t_entry_data *entry_data)
{
	struct group	*group_info;

	if (args->id_cache.last_gid != entry_data->stat_buf.st_gid)
	{
		group_info = getgrgid(entry_data->stat_buf.st_gid);
		args->id_cache.last_gid = entry_data->stat_buf.st_gid;
		if (group_info)
			args->id_cache.last_group_name = group_info->gr_name;
		else
			args->id_cache.last_group_name = NULL;
	}
}

// Counts the number of digits in a 64-bit unsigned integer. Returns the count
// as an 8-bit unsigned integer. Used to determine the width of the file size
// field and hard links field for the long listing format.
uint8_t	count_number_digits(uint64_t number)
{
	uint8_t	count;

	if (number == 0)
		return (1);
	count = 0;
	while (number > 0)
	{
		number /= 10;
		count++;
	}
	return (count);
}

// Prints 'spaces' number of blank spaces to align the output in the long
// listing format.
void	print_blanks(uint8_t spaces)
{
	for (uint8_t i = 0; i < spaces; i++)
		ft_printf(" ");
}

// Analyzes the whole list of entries, to determine the maximum widths for each
// one of the following fields (file size, number of links, user name and group
// name). Returns a pointer to an allocated t_widths structure containing the
// maximum width for each field.
t_widths	*calculate_fields_widths(t_args *args, t_list *entries_list)
{
	t_widths		*field_widths;
	t_entry_data	*entry_data;
	t_list			*list;

	field_widths = malloc(sizeof(t_widths));
	ft_bzero(field_widths, sizeof(t_widths));
	list = entries_list;
	while (list)
	{
		entry_data = (t_entry_data *)list->content;
		if (entry_data->stat_buf.st_size > field_widths->largest_size)
			field_widths->largest_size = entry_data->stat_buf.st_size;
		if (entry_data->stat_buf.st_nlink > field_widths->largest_nlink)
			field_widths->largest_nlink = entry_data->stat_buf.st_nlink;
		if (!args->hide_owner)
		{
			check_cached_user_name(args, entry_data);
			if (args->id_cache.last_user_name && \
			ft_strlen(args->id_cache.last_user_name) > field_widths->user_w)
				field_widths->user_w = ft_strlen(args->id_cache.last_user_name);
		}
		if (!args->hide_group)
		{
			check_cached_group_name(args, entry_data);
			if (args->id_cache.last_group_name && \
			ft_strlen(args->id_cache.last_group_name) > field_widths->group_w)
				field_widths->group_w = ft_strlen(args->id_cache.last_group_name);
		}
		list = list->next;
	}
	field_widths->size_w = count_number_digits(field_widths->largest_size);
	field_widths->nlink_w = count_number_digits(field_widths->largest_nlink);
	return (field_widths);
}
