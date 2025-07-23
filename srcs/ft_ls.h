/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:58:48 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/23 17:55:40 by jesuserr         ###   ########.fr       */
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
# include <time.h>					// for ctime, time
# include <pwd.h>					// for getpwuid struct
# include <grp.h>					// for getgrgid struct

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define SECONDS_IN_6_MONTHS	(6 * 30 * 24 * 60 * 60)  // 15,552,000 seconds
# define CTIME_SIZE				26	// size of ctime string, including '\0'
# define SKIP_DAY				4	// characters to skip day in ctime string
# define PERMISSIONS_SIZE		11	// size permissions string, including '\0'

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
void		list_files(t_args *args, const char *current_path);
char		*build_full_path(const char *curr_pth, const struct dirent *entry);
void		list_dirs(t_args *args, const char *current_path);

/********************************** main.c ************************************/
void		free_allocated_memory(t_args *args);

/********************************** merge_sort_comp.c *************************/
int			compare_names_cli(const void *a, const void *b, bool reverse);
int			compare_times_cli(const void *a, const void *b, bool reverse);
int			compare_d_names(const void *a, const void *b, bool reverse);
int			compare_stat_times(const void *a, const void *b, bool reverse);

/********************************** merge_sort.c ******************************/
void		sort_list(t_list **list, t_compare_func compare, bool reverse);

/********************************** parser.c **********************************/
void		parse_arguments(char **argv, t_args *args);

/********************************** print_list.c ******************************/
void		print_file_name(t_entry_data *entry_data, const char *current_path, \
			bool long_format);
void		print_list(t_args *args, t_list *entries_lst, const char *curr_pth);

/********************************** print_long_format.c ***********************/
void		print_long_line(t_entry_data *entry_data, const char *current_path);
void		print_long_format(t_list *entries_list, const char *current_path);

#endif
