/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_long_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:38:38 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/22 14:03:08 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Function needed because my ft_printf does not support 'size_t' printing.
// It is assumed that 'nbr' will be 64-bit unsigned integer and therefore, 20
// digits will be enough to represent its maximum value.
static void	print_size_t_as_digits(uint64_t nbr)
{
	uint8_t	digits[20];
	uint8_t	i;
	bool	leading_zero;

	if (nbr == 0)
	{
		ft_printf("0");
		return ;
	}
	i = 0;
	while (i < 20)
		digits[i++] = 0;
	i = 19;
	digits [i--] = nbr % 10;
	nbr = nbr / 10;
	while (nbr >= 10)
	{
		digits[i--] = nbr % 10;
		nbr = nbr / 10;
	}
	digits[i--] = (int)nbr;
	leading_zero = true;
	while (i < 20)
	{
		while (digits[i] == 0 && leading_zero)
			i++;
		ft_printf("%d", digits[i++]);
		leading_zero = false;
	}
}

// Calculates the total number of 1K blocks used by the files in the list.
// st_blocks is in 512-byte blocks, so we divide by 2 to convert to 1K blocks.
// Only counts printed files and this amount represents actual disk usage, not
// just file sizes.
uint64_t	calculate_total_blocks(t_args *args, t_list *entries_list)
{
	t_entry_data	*entry_data;
	uint64_t		total_blocks;

	total_blocks = 0;
	while (entries_list)
	{
		entry_data = (t_entry_data *)entries_list->content;
		if (entry_data->entry.d_name[0] == '.' && !args->all)
		{
			entries_list = entries_list->next;
			continue ;
		}
		total_blocks += entry_data->stat_buf.st_blocks;
		entries_list = entries_list->next;
	}
	return (total_blocks / 2);
}

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

// Prints the long listing format for a single file entry.
void	print_long_listing(t_entry_data *entry_data)
{
	struct passwd	*user_info;
	struct group	*group_info;
	char			*formatted_time;

	user_info = getpwuid(entry_data->stat_buf.st_uid);
	group_info = getgrgid(entry_data->stat_buf.st_gid);
	printf("%o ", entry_data->stat_buf.st_mode & 0777);
	fflush(stdout);
	print_size_t_as_digits(entry_data->stat_buf.st_nlink);
	ft_printf("\t");
	if (user_info)
		ft_printf(" %s ", user_info->pw_name);
	else
		ft_printf(" %d ", entry_data->stat_buf.st_uid);
	if (group_info)
		ft_printf("%s ", group_info->gr_name);
	else
		ft_printf("%d ", entry_data->stat_buf.st_gid);
	print_size_t_as_digits(entry_data->stat_buf.st_size);
	formatted_time = get_formatted_time(&entry_data->stat_buf);
	ft_printf(" %s", formatted_time);
	ft_printf(" %s \n", entry_data->entry.d_name);
	free(formatted_time);
}

// make && ./ft_ls /mnt/g/Videos/Documentales/Blue\ Planet\ II -l