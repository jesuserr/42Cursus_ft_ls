/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:45 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/10 22:46:28 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// First of all verifies is '--help' is present along all arguments to not
// allocate anything and just print the help message and exit.
void	parse_arguments(char **argv, t_ls_data *ls_data)
{
	uint8_t	i;

	i = 0;
	while (argv[++i])
	{
		if (!ft_strncmp(argv[i], "--help", 6) && ft_strlen(argv[i]) == 6)
			print_usage();
	}
	i = 0;
	while (argv[++i])
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0' && ft_strlen(argv[i]) < CHAR_MAX)
		{
			for (uint8_t j = 1; argv[i][j]; j++)
			{
				if (argv[i][j] == 'a')
					ls_data->args.all = true;
				else if (argv[i][j] == 'l')
					ls_data->args.long_listing = true;
				else if (argv[i][j] == 'r')
					ls_data->args.reverse = true;
				else if (argv[i][j] == 'R')
					ls_data->args.recursive = true;
				else if (argv[i][j] == 't')
					ls_data->args.sort_by_time = true;
				else
					print_invalid_option_and_exit(argv[i][j], ls_data);
			}
		}
		else
			ft_printf("File: %s\n", argv[i]);
	}
}
