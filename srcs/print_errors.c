/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:49 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/17 23:44:11 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_invalid_option_and_exit(const char option, t_ls_data *ls_data)
{
	ft_putstr_fd("ft_ls: invalid option -- '", STDERR_FILENO);
	ft_putchar_fd(option, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	ft_putstr_fd("Try 'ft_ls --help' for more information.\n", STDERR_FILENO);
	free_allocated_memory(ls_data);
	exit (EXIT_FAILURE);
}

void	print_perror_and_exit(const char *msg, t_ls_data *ls_data)
{
	perror(msg);
	free_allocated_memory(ls_data);
	exit(EXIT_FAILURE);
}

void	print_usage(void)
{
	ft_printf("Usage:\n"
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
