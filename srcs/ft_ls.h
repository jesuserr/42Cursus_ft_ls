/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:58:48 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/20 13:25:47 by jesuserr         ###   ########.fr       */
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
// Function pointer type for comparison functions. Creates a type called
// t_compare_func that points to a function that takes two const void pointers
// and boolean reverse as arguments and returns an int.
// typedef for function pointers is a best practice that makes code more
// readable, reusable, maintainable, and less error-prone.
typedef int	(*t_compare_func)(const void *a, const void *b, bool reverse);

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
	t_list		*cli_files_list;	// linked list of files to 'ls'
	t_list		*cli_dirs_list;		// linked list of directories to 'ls'
	bool		first_printing;		// used to not print "\n" the first time
}	t_args;

typedef struct s_entry_data
{
	struct dirent	entry;			// dirent struct to hold file name and type
	struct stat		stat_buf;		// stat struct to hold file metadata
}	t_entry_data;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/
/********************************** list_files.c ******************************/
void	list_files(t_args *args, const char *current_path);

/********************************** main.c ************************************/
void	free_allocated_memory(t_args *args);

/********************************** merge_sort_comp.c *************************/
int		compare_names_cli(const void *a, const void *b, bool reverse);
int		compare_times_cli(const void *a, const void *b, bool reverse);
int		compare_d_names(const void *a, const void *b, bool reverse);
int		compare_stat_times(const void *a, const void *b, bool reverse);

/********************************** merge_sort.c ******************************/
void	sort_list(t_list **list, t_compare_func compare, bool reverse);

/********************************** parser.c **********************************/
void	parse_arguments(char **argv, t_args *args);

/********************************** print_errors.c ****************************/
void	print_invalid_option_and_exit(const char option, t_args *args);
void	print_perror_and_exit(const char *msg, t_args *args);
void	print_usage(void);

/********************************** print_list.c ******************************/
void	print_list_aux(t_list *list);
void	print_list_aux2(t_list *list);
void	print_list(t_args *args, t_list *entries_list, const char *curr_path);

#endif
