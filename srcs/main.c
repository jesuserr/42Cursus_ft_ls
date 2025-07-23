/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:33 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/23 21:45:10 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Traverses the linked list of files, calling list_files().
static void	files_listing(t_args *args)
{
	t_list	*list;

	list = args->cli_files_list;
	while (list)
	{
		list_files(args, (char *)list->content);
		list = list->next;
		if (!list && !args->long_listing)
			ft_printf("\n");
	}
}

// Traverses the linked list of directories, calling list_dirs().
static void	dirs_listing(t_args *args)
{
	t_list	*list;

	list = args->cli_dirs_list;
	while (list)
	{
		list_dirs(args, (char *)list->content);
		list = list->next;
	}
}

// Frees the memory allocated during program execution.
void	free_allocated_memory(t_args *args)
{
	if (args->cli_files_list)
		ft_lstclear(&args->cli_files_list, free);
	if (args->cli_dirs_list)
		ft_lstclear(&args->cli_dirs_list, free);
	return ;
}

int	main(int argc, char **argv)
{
	t_args	args;

	(void)argc;
	ft_bzero(&args, sizeof(t_args));
	parse_arguments(argv, &args);
	files_listing(&args);
	dirs_listing(&args);
	free_allocated_memory(&args);
	return (EXIT_SUCCESS);
}
