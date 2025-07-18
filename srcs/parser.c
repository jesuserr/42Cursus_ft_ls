/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/18 00:33:47 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// First of all verifies if '--help' is present along all arguments to not
// allocate anything and just print the help message and exit.
// All arguments that are not options (starting with '-') and exist as file or
// directory are added to linked list of files/directories to 'ls' and then the
// list is sorted alphabetically. lstat() is used to check if the file exists
// as the actual ls command does, which means it will show broken symlinks in
// the listing.
void	parse_arguments(char **argv, t_args *args)
{
	struct stat	test_file_exist;
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
					args->all = true;
				else if (argv[i][j] == 'l')
					args->long_listing = true;
				else if (argv[i][j] == 'r')
					args->reverse = true;
				else if (argv[i][j] == 'R')
					args->recursive = true;
				else if (argv[i][j] == 't')
					args->sort_by_time = true;
				else
					print_invalid_option_and_exit(argv[i][j], args);
			}
		}
		else
		{
			if (lstat(argv[i], &test_file_exist) == 0)
				ft_lstadd_back(&args->cli_files_list, ft_lstnew(ft_strdup(argv[i])));
			else
			{
				ft_printf("ft_ls: cannot access '%s': ", argv[i]);
				perror("");
				no_such_file = true;
			}
		}
	}
	if (args->cli_files_list)
		sort_list(&args->cli_files_list);
	else if (!no_such_file)
		ft_lstadd_back(&args->cli_files_list, ft_lstnew(ft_strdup(".")));
	//ft_hex_dump(args, sizeof(t_args), 8);
	//ft_printf("List size: %d\n", ft_lstsize(args->cli_files_list));
}
