/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:58:48 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/15 11:57:11 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/
# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/ft_printf.h"
# include <dirent.h>				// for opendir, readdir, closedir
# include <sys/stat.h>				// for stat, lstat
# include <stdbool.h>				// for booleans
# include <stdint.h>				// for fixed-width integer types
# include <time.h>					// for time

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_args
{
	bool		all;				// option -a
	bool		long_listing;		// option -l
	bool		reverse;			// option -r
	bool		recursive;			// option -R
	bool		sort_by_time;		// option -t
	t_list		*cli_files_list;	// linked list of files/directories to 'ls'
}	t_args;

typedef struct s_ls_data
{
	t_args		args;
}	t_ls_data;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/
/********************************** bonus.c ***********************************/

/********************************** main.c ************************************/
void	free_allocated_memory(t_ls_data *ls_data);
/********************************** merge_sort.c ******************************/
void	sort_list(t_list **list);

/********************************** parser.c **********************************/
void	parse_arguments(char **argv, t_ls_data *ls_data);

/********************************** print_errors.c ****************************/
void	print_invalid_option_and_exit(char option, t_ls_data *ls_data);
void	print_perror_and_exit(char *msg, t_ls_data *ls_data);
void	print_usage(void);

#endif
