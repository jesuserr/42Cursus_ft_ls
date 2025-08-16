/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:58:48 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/16 14:01:13 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/

# include "../libft/includes/libft.h"
# include <dirent.h>				// for opendir, readdir, closedir
# include <sys/stat.h>				// for stat, lstat
# include <stdbool.h>				// for booleans
# include <stdint.h>				// for fixed-width integer types
# include <time.h>					// for ctime, time
# include <pwd.h>					// for getpwuid struct
# include <grp.h>					// for getgrgid struct
# include <stdarg.h>				// for va_list, va_start, va_end
# include <sys/ioctl.h>				// for ioctl, TIOCGWINSZ (bonus)
# include <stdio.h>					// for perror
# include <sys/xattr.h>				// for listxattr (ACL and xattr) (bonus)

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

// Cache structure to store user and group info to avoid repeated calls to
// getpwuid() and getgrgid(), which are very expensive in terms of memory and
// performance due to their access to complex system databases to retrieve
// user and group names based on IDs. Performance is improved by caching the
// last user and group IDs and their corresponding names.
typedef struct s_id_cache
{
	uid_t	last_uid;			// last user ID cached
	char	*last_user_name;	// last user name cached
	gid_t	last_gid;			// last group ID cached
	char	*last_group_name;	// last group name cached
}	t_id_cache;

typedef struct s_args
{
	bool		all;				// option -a
	bool		almost_all;			// option -A (bonus)
	bool		no_colors;			// option -c (bonus - non-canonical option)
	bool		hide_owner;			// option -g (bonus)
	bool		human_readable;		// option -h (bonus)
	bool		inode;				// option -i (bonus - only works with -l)
	bool		long_listing;		// option -l
	bool		quoting;			// option -N (bonus - as 'ls' but reversed)
	bool		hide_group;			// option -o (bonus)
	bool		total_files;		// option -q (bonus - non-canonical option)
	bool		reverse;			// option -r
	bool		recursive;			// option -R
	bool		sort_by_size;		// option -S (bonus)
	bool		sort_by_time;		// option -t
	bool		tab_separated;		// option -T (bonus - non-canonical option)
	bool		no_sort;			// option -U (bonus)
	bool		acl_and_xattr;		// option -x (bonus - non-canonical option)
	bool		one_file_per_line;	// option -1 (bonus)
	t_list		*cli_files_list;	// linked list of files to 'ls'
	t_list		*cli_dirs_list;		// linked list of directories to 'ls'
	bool		first_printing;		// used to not print "\n" the first time
	uint8_t		exit_status;		// exit status of the program
	t_id_cache	id_cache;			// cache for user/group info
	char		separator[3];		// separator for tab-separated output
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
	uint8_t		inode_w;			// max width for inode number field
	int64_t		largest_size;		// largest file size in the list
	uint64_t	largest_nlink;		// largest number of hard links in the list
	uint64_t	largest_inode;		// largest inode number in the list
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
void		check_cached_user_name(t_args *args, t_entry_data *entry_data);
void		check_cached_group_name(t_args *args, t_entry_data *entry_data);
uint8_t		count_number_digits(uint64_t number);
void		print_blanks(uint8_t spaces);
t_widths	*calculate_fields_widths(t_args *args, t_list *entries_list);
void		print_human_readable_size(t_args *args, uint64_t size);

/********************************** print_printf.c ***************************/
void		ft_printf(char const *hold, ...);
void		ft_printf_flush(void);

#endif
