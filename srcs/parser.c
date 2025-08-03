/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/08/03 23:46:38 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	print_invalid_option_and_exit(const char option, t_args *args)
{
	ft_putstr_fd("ft_ls: invalid option -- '", STDERR_FILENO);
	ft_putchar_fd(option, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	ft_putstr_fd("Try 'ft_ls --help' for more information.\n", STDERR_FILENO);
	free_allocated_memory(args);
	exit(EXIT_SERIOUS_ERROR);
}

static void	print_usage(void)
{
	ft_printf("Usage:\n"
		"  ft_ls [OPTION]... [FILE]...\n\n"
		"Options:\n"
		"  --help                  display this help and exit\n"
		"  -a                      do not ignore entries starting with .\n"
		"  -A                      do not list implied . and ..\n"
		"  -g                      like -l, but do not list owner\n"
		"  -l                      use a long listing format\n"
		"  -N                      print entry names with quoting\n"
		"  -o                      like -l, but do not list group information\n"
		"  -q                      display total number of entries\n"
		"  -r                      reverse order while sorting\n"
		"  -R                      list subdirectories recursively\n"
		"  -S                      sort by file size, largest first\n"
		"  -t                      sort by time, newest first\n"
		"  -x                      list ACL and extended attributes\n"
		"  -1                      list one file per line\n\n"
		"List information about the FILEs (the current directory by default).\n"
		"Sort entries alphabetically if none of -t or -S is specified.\n\n"
		"Exit status:\n"
		"  0  if OK,\n"
		"  1  if minor problems (e.g., cannot access subdirectory),\n"
		"  2  if serious trouble (e.g., cannot access command-line argument).\n"
		"\nGitHub repository: <https://github.com/jesuserr/42Cursus_ft_ls>\n");
	ft_printf_flush();
	exit(EXIT_SUCCESS);
}

// Uses lstat() to check if entity exists and determine its type. Directories
// are added to cli_dirs_list, while anything else is added to cli_files_list.
// If the entity doesn't exist or cannot be access, prints an error message, 
// sets no_such_file flag and exit_status to proper values to indicate failure
// and continues. lstat() is used to check if the file exists as the actual ls
// command does, which means it will show broken symlinks in the listing.
static void	add_entity_to_list(char *entity, t_args *args, bool *no_such_file)
{
	struct stat	file_exists;
	DIR			*dir;

	if (lstat(entity, &file_exists) == 0)
	{
		if (S_ISDIR(file_exists.st_mode))
		{
			dir = opendir(entity);
			if (!dir)
			{
				ft_putstr_fd("ft_ls: cannot open directory '", STDERR_FILENO);
				ft_putstr_fd(entity, STDERR_FILENO);
				ft_putstr_fd("': ", STDERR_FILENO);
				perror("");
				args->exit_status = EXIT_SERIOUS_ERROR;
				*no_such_file = true;
			}
			else
			{
				ft_lstadd_back(&args->cli_dirs_list, ft_lstnew(ft_strdup(entity)));
				closedir(dir);
			}
		}
		else
			ft_lstadd_back(&args->cli_files_list, ft_lstnew(ft_strdup(entity)));
	}
	else
	{
		ft_putstr_fd("ft_ls: cannot access '", STDERR_FILENO);
		ft_putstr_fd(entity, STDERR_FILENO);
		ft_putstr_fd("': ", STDERR_FILENO);
		perror("");
		args->exit_status = EXIT_SERIOUS_ERROR;
		*no_such_file = true;
	}
}

static void	sort_cli_lists(t_args *args)
{
	if (args->cli_files_list && args->sort_by_time)
		sort_list(&args->cli_files_list, compare_times_cli, args->reverse);
	else if (args->cli_files_list && args->sort_by_size)
		sort_list(&args->cli_files_list, compare_sizes_cli, args->reverse);
	else if (args->cli_files_list)
		sort_list(&args->cli_files_list, compare_names_cli, args->reverse);
	if (args->cli_dirs_list && args->sort_by_time)
		sort_list(&args->cli_dirs_list, compare_times_cli, args->reverse);
	else if (args->cli_dirs_list && args->sort_by_size)
		sort_list(&args->cli_dirs_list, compare_sizes_cli, args->reverse);
	else if (args->cli_dirs_list)
		sort_list(&args->cli_dirs_list, compare_names_cli, args->reverse);
}

// First of all verifies if '--help' is present along all arguments to not
// allocate anything and just print the help message and exit.
// All arguments that are not options (starting with '-') are verified to
// exist as files or directories. At the end, lists are sorted depending on
// the options set by the user.
void	parse_arguments(char **argv, t_args *args)
{
	bool		no_such_file;

	no_such_file = false;
	for (uint8_t i = 1; argv[i]; i++)
		if (!ft_strncmp(argv[i], "--help", 6) && ft_strlen(argv[i]) == 6)
			print_usage();
	for (uint8_t i = 1; argv[i]; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			for (uint8_t j = 1; argv[i][j]; j++)
			{
				if (argv[i][j] == 'a')
				{
					args->all = true;
					args->almost_all = false;
				}
				else if (argv[i][j] == 'A')
				{
					args->almost_all = true;
					args->all = false;
				}
				else if (argv[i][j] == 'g')
					args->hide_owner = true;
				else if (argv[i][j] == 'l')
					args->long_listing = true;
				else if (argv[i][j] == 'N')
					args->quoting = true;
				else if (argv[i][j] == 'o')
					args->hide_group = true;
				else if (argv[i][j] == 'q')
					args->total_files = true;
				else if (argv[i][j] == 'r')
					args->reverse = true;
				else if (argv[i][j] == 'R')
					args->recursive = true;
				else if (argv[i][j] == 'S')
				{
					args->sort_by_size = true;
					args->sort_by_time = false;
				}
				else if (argv[i][j] == 't')
				{
					args->sort_by_time = true;
					args->sort_by_size = false;
				}
				else if (argv[i][j] == 'x')
					args->acl_and_xattr = true;
				else if (argv[i][j] == '1')
					args->one_file_per_line = true;
				else
					print_invalid_option_and_exit(argv[i][j], args);
			}
		}
		else
			add_entity_to_list(argv[i], args, &no_such_file);
	}
	if (args->cli_dirs_list || args->cli_files_list)
		sort_cli_lists(args);
	if (!no_such_file && !args->cli_dirs_list && !args->cli_files_list)
		ft_lstadd_back(&args->cli_dirs_list, ft_lstnew(ft_strdup(".")));
	args->first_printing = true;
	if (args->hide_group || args->hide_owner)
		args->long_listing = true;
}
