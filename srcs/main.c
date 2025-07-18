/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:33 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/18 21:21:31 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Frees the memory allocated during program execution.
void	free_allocated_memory(t_args *args)
{
	if (args->cli_files_list)
		ft_lstclear(&args->cli_files_list, free);
	return ;
}

int	main(int argc, char **argv)
{
	t_args		args;
	t_list		*files_list;

	(void)argc;
	ft_bzero(&args, sizeof(t_args));
	parse_arguments(argv, &args);
	files_list = args.cli_files_list;
	while (files_list)
	{
		list_files(&args, (char *)files_list->content);
		files_list = files_list->next;
	}
	free_allocated_memory(&args);
	return (EXIT_SUCCESS);
}
