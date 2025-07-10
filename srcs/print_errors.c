/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:49 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/10 22:46:37 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_invalid_option_and_exit(char option, t_ls_data *ls_data)
{
	fprintf(stderr, "ft_ls: invalid option -- '%c'\n", option);
	fprintf(stderr, "Try 'ft_ls --help' for more information.\n");
	free_allocated_memory(ls_data);
	exit (EXIT_FAILURE);
}

void	print_perror_and_exit(char *msg, t_ls_data *ls_data)
{
	perror(msg);
	free_allocated_memory(ls_data);
	exit(EXIT_FAILURE);
}

void	print_usage(void)
{
	printf("Usage:\n"
		"  ft_ls [OPTION]... [FILE]...\n\n"
		"Options:\n"
		"  --help                  display this help and exit\n"
		"  -a                      do not ignore entries starting with .\n"
		"  -l                      use a long listing format\n"
		"  -r                      reverse order while sorting\n"
		"  -R                      list subdirectories recursively\n"
		"  -t                      sort by time, newest first\n");
	exit(EXIT_SUCCESS);
}
