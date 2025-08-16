/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_long_format.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:38:38 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/16 13:30:45 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Calculates the total number of 1K blocks used by the files in the list.
// st_blocks is in 512-byte blocks, so we divide by 2 to convert to 1K blocks.
// Only counts printed files and this amount represents actual disk usage, not
// just file sizes.
static uint64_t	calculate_total_blocks(t_list *entries_list)
{
	t_entry_data	*entry_data;
	uint64_t		total_blocks;

	total_blocks = 0;
	while (entries_list)
	{
		entry_data = (t_entry_data *)entries_list->content;
		total_blocks += entry_data->stat_buf.st_blocks;
		entries_list = entries_list->next;
	}
	return (total_blocks / 2);
}

// Formats file modification time for long output. Returns "Mon DD HH:MM" format
// for recent files (within 6 months) or "Mon DD  YYYY" for older files. Uses
// ctime() string manipulation to extract month, day, and time/year components.
// Returns dynamically allocated string that caller must free.
static char	*get_formatted_time(struct stat *stat_buf)
{
	char	*ctime_time;
	char	ctime_copy[CTIME_SIZE];
	time_t	current_time;

	current_time = time(NULL);
	if (current_time == (time_t)-1)
		return (NULL);
	ctime_time = ctime(&stat_buf->st_mtime);
	if (ctime_time == NULL)
		return (NULL);
	ft_strlcpy(ctime_copy, ctime_time, sizeof(ctime_copy));
	if (current_time - stat_buf->st_mtime > SECONDS_IN_6_MONTHS)
		ft_strlcpy(ctime_copy + 11, ctime_copy + 19, 6);
	ctime_copy[16] = '\0';
	return (ft_strdup(ctime_copy + SKIP_DAY));
}

// Prints file type and permission string in ls -l format. First character shows
// file type (-, d, l, etc.). Following nine characters show owner, group, and
// other permissions (rwx). Handles special bits: setuid/setgid (s/S) and sticky
// bit (t/T). Uppercase indicates special bit set without execute permission.
static void	print_file_permissions(mode_t mode)
{
	char	perms[PERMISSIONS_SIZE];

	if (S_ISREG(mode))
		perms[0] = '-';
	else if (S_ISDIR(mode))
		perms[0] = 'd';
	else if (S_ISCHR(mode))
		perms[0] = 'c';
	else if (S_ISBLK(mode))
		perms[0] = 'b';
	else if (S_ISFIFO(mode))
		perms[0] = 'p';
	else if (S_ISSOCK(mode))
		perms[0] = 's';
	else if (S_ISLNK(mode))
		perms[0] = 'l';
	else
		perms[0] = '?';
	perms[1] = (mode & S_IRUSR) ? 'r' : '-';
	perms[2] = (mode & S_IWUSR) ? 'w' : '-';
	if (mode & S_ISUID)
		perms[3] = (mode & S_IXUSR) ? 's' : 'S';
	else
		perms[3] = (mode & S_IXUSR) ? 'x' : '-';
	perms[4] = (mode & S_IRGRP) ? 'r' : '-';
	perms[5] = (mode & S_IWGRP) ? 'w' : '-';
	if (mode & S_ISGID)
		perms[6] = (mode & S_IXGRP) ? 's' : 'S';
	else
		perms[6] = (mode & S_IXGRP) ? 'x' : '-';
	perms[7] = (mode & S_IROTH) ? 'r' : '-';
	perms[8] = (mode & S_IWOTH) ? 'w' : '-';
	if (mode & S_ISVTX)
		perms[9] = (mode & S_IXOTH) ? 't' : 'T';
	else
		perms[9] = (mode & S_IXOTH) ? 'x' : '-';
	perms[10] = '\0';
	ft_printf("%s", perms);
}

// Prints the long listing format for a single file entry.
// The Linux Programming Interface - Chapter 15
void	print_long_line(t_args *args, t_entry_data *entry_data, \
const char *current_path, t_widths *widths)
{
	char	*formatted_time;
	char	*full_path;

	if (args->inode)
	{
		print_blanks(widths->inode_w - count_number_digits(entry_data->stat_buf.st_ino));
		ft_printf("%d", entry_data->stat_buf.st_ino);
		ft_printf("%s", args->separator);
	}
	print_file_permissions(entry_data->stat_buf.st_mode);
	if (args->acl_and_xattr)
	{
		full_path = build_full_path(current_path, &entry_data->entry);
		if (listxattr(full_path, NULL, 0) > 0)
			ft_printf("+");
		else
			ft_printf(" ");
		free(full_path);
	}
	print_blanks(widths->nlink_w - count_number_digits(entry_data->stat_buf.st_nlink));
	ft_printf("%s", args->separator);
	ft_printf("%d%s", entry_data->stat_buf.st_nlink, args->separator);
	if (!args->hide_owner)
	{
		check_cached_user_name(args, entry_data);
		if (args->id_cache.last_user_name)
		{
			ft_printf("%s%s", args->id_cache.last_user_name, args->separator);
			print_blanks(widths->user_w - ft_strlen(args->id_cache.last_user_name));
		}
		else
			ft_printf("%d%s", entry_data->stat_buf.st_uid, args->separator);
	}
	if (!args->hide_group)
	{
		check_cached_group_name(args, entry_data);
		if (args->id_cache.last_group_name)
		{
			ft_printf("%s%s", args->id_cache.last_group_name, args->separator);
			print_blanks(widths->group_w - ft_strlen(args->id_cache.last_group_name));
		}
		else
			ft_printf("%d%s", entry_data->stat_buf.st_gid, args->separator);
	}
	if (!args->human_readable)
	{
		print_blanks(widths->size_w - count_number_digits(entry_data->stat_buf.st_size));
		ft_printf("%d%s", entry_data->stat_buf.st_size, args->separator);
	}
	else
		print_human_readable_size(args, entry_data->stat_buf.st_size);
	formatted_time = get_formatted_time(&entry_data->stat_buf);
	ft_printf("%s%s", formatted_time, args->separator);
	print_file_name(args, entry_data, current_path);
	ft_printf("\n");
	free(formatted_time);
}

// Displays directory contents in detailed long format. Prints total disk usage
// header, calculates maximum column widths and then iterates through entries
// printing each file's detailed information via print_long_line().
// Frees allocated width structure.
void	print_long_format(t_args *args, t_list *entries_list, const char *current_path)
{
	t_entry_data	*entry_data;
	t_widths		*widths;

	ft_printf("total ");
	if (!args->human_readable)
		ft_printf("%d", calculate_total_blocks(entries_list));
	else
		print_human_readable_size(args, calculate_total_blocks(entries_list) * 1024);
	ft_printf("\n");
	widths = calculate_fields_widths(args, entries_list);
	while (entries_list)
	{
		entry_data = (t_entry_data *)entries_list->content;
		print_long_line(args, entry_data, current_path, widths);
		entries_list = entries_list->next;
	}
	free(widths);
}
