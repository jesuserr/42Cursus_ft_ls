/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:58:48 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/29 21:32:02 by jesuserr         ###   ########.fr       */
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
# include <dirent.h>				// for opendir, readdir, closedir
# include <sys/stat.h>				// for stat, lstat
# include <stdbool.h>				// for booleans
# include <stdint.h>				// for fixed-width integer types
# include <time.h>					// for ctime, time
# include <pwd.h>					// for getpwuid struct
# include <grp.h>					// for getgrgid struct
# include <stdarg.h>				// for va_list, va_start, va_end
# include <sys/ioctl.h>				// for ioctl, TIOCGWINSZ (bonus)

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define SECONDS_IN_6_MONTHS	(6 * 30 * 24 * 60 * 60)  // 15,552,000 seconds
# define CTIME_SIZE				26	// size of ctime string, including '\0'
# define SKIP_DAY				4	// characters to skip day in ctime string
# define PERMISSIONS_SIZE		11	// size permissions string, including '\0'
# define EXIT_MINOR_ERROR		1	// exit status for minor problems
# define EXIT_SERIOUS_ERROR		2	// exit status for serious troubles
# define BUFFER_SIZE_PRINTF		8192	// size of the ft_printf print buffer
# define COLOR_RESET			"\033[0m"		// reset color to default
# define COLOR_DIRECTORY		"\033[01;34m"	// bold blue
# define COLOR_SYMLINK			"\033[01;36m"	// bold cyan
# define COLOR_EXECUTABLE		"\033[01;32m"	// bold green
# define COLOR_SOCKET			"\033[01;35m"	// bold magenta
# define COLOR_NAMED_PIPE		"\033[40;33m"	// yellow on black
# define COLOR_DEVICE			"\033[01;37m"	// bold white
# define COLOR_STICKY_DIR		"\033[30;42m"	// black on green
# define COLOR_SETGID_FILE		"\033[30;43m"	// black on yellow
# define COLOR_SETUID_FILE		"\033[30;41m"	// black on red

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
	bool		almost_all;			// option -A (bonus)
	bool		hide_owner;			// option -g (bonus)
	bool		long_listing;		// option -l
	bool		hide_group;			// option -o (bonus)
	bool		total_files;		// option -q (bonus - non-canonical option)
	bool		reverse;			// option -r
	bool		recursive;			// option -R
	bool		sort_by_size;		// option -S (bonus)
	bool		sort_by_time;		// option -t
	bool		one_file_per_line;	// option -1 (bonus)
	t_list		*cli_files_list;	// linked list of files to 'ls'
	t_list		*cli_dirs_list;		// linked list of directories to 'ls'
	bool		first_printing;		// used to not print "\n" the first time
	uint8_t		exit_status;		// exit status of the program
}	t_args;

typedef struct s_entry_data
{
	struct dirent	entry;			// dirent struct to hold file name and type
	struct stat		stat_buf;		// stat struct to hold file metadata
}	t_entry_data;

typedef struct s_widths
{
	uint8_t		size_w;				// max width for file size field
	uint8_t		nlink_w;			// max width for number of links field
	uint8_t		user_w;				// max width for user name field
	uint8_t		group_w;			// max width for group name field
	int64_t		largest_size;		// largest file size in the list
	uint64_t	largest_nlink;		// largest number of hard links in the list
}	t_widths;

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/
/********************************** list_routines.c ***************************/
void		list_files(t_args *args, t_list *entries_list, t_widths *widths);
void		list_dirs(t_args *args, const char *current_path);

/********************************** main.c ************************************/
void		free_allocated_memory(t_args *args);

/********************************** merge_sort_comp.c *************************/
int			compare_names_cli(const void *a, const void *b, bool reverse);
int			compare_times_cli(const void *a, const void *b, bool reverse);
int			compare_sizes_cli(const void *a, const void *b, bool reverse);
int			compare_d_names(const void *a, const void *b, bool reverse);
int			compare_stat_times(const void *a, const void *b, bool reverse);
int			compare_stat_sizes(const void *a, const void *b, bool reverse);

/********************************** merge_sort.c ******************************/
void		sort_list(t_list **list, t_compare_func compare, bool reverse);

/********************************** parser.c **********************************/
void		parse_arguments(char **argv, t_args *args);

/********************************** print_list.c ******************************/
char		*build_full_path(const char *curr_pth, const struct dirent *entry);
void		print_file_name(t_args *args, t_entry_data *entry_data, \
			const char *current_path);
void		print_list(t_args *args, t_list *entries_lst, const char *curr_pth);

/********************************** print_long_format.c ***********************/
void		print_long_line(t_args *args, t_entry_data *entry_data, \
			const char *current_path, t_widths *widths);
void		print_long_format(t_args *args, t_list *entries_list, \
			const char *current_path);

/********************************** print_long_utils.c ************************/
uint8_t		count_number_digits(uint64_t number);
void		print_blanks(uint8_t spaces);
t_widths	*calculate_fields_widths(t_args *args, t_list *entries_list);

/********************************** print_printf.c ***************************/
void		ft_printf(char const *hold, ...);
void		ft_printf_flush(void);

#endif
