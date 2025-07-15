/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/15 12:45:11 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// First of all verifies is '--help' is present along all arguments to not
// allocate anything and just print the help message and exit.
// All arguments that are not options (starting with '-') are added to linked
// list of files/directories to 'ls' and then list is sorted alphabetically.
void	parse_arguments(char **argv, t_ls_data *ls_data)
{
	t_args	*args;

	args = &ls_data->args;
	for (uint8_t i = 1; argv[i]; i++)
		if (!ft_strncmp(argv[i], "--help", 6) && ft_strlen(argv[i]) == 6)
			print_usage();
	for (uint8_t i = 1; argv[i]; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0' && ft_strlen(argv[i]) < CHAR_MAX)
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
					print_invalid_option_and_exit(argv[i][j], ls_data);
			}
		}
		else
			ft_lstadd_back(&args->cli_files_list, ft_lstnew(ft_strdup(argv[i])));
	}
	if (args->cli_files_list)
		sort_list(&args->cli_files_list);
}
