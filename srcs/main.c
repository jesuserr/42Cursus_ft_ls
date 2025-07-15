/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:33 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/15 12:04:37 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Auxiliar function for debugging purposes.
void	print_list(t_list *list)
{
	while (list)
	{
		ft_printf("Content: %s\n", (char *)list->content);
		list = list->next;
	}
}

// Frees the memory allocated during program execution.
void	free_allocated_memory(t_ls_data *ls_data)
{
	if (ls_data->args.cli_files_list)
		ft_lstclear(&ls_data->args.cli_files_list, free);
	return ;
}

int	main(int argc, char **argv)
{
	t_ls_data	ls_data;

	(void)argc;
	ft_bzero(&ls_data, sizeof(t_ls_data));
	parse_arguments(argv, &ls_data);
	
	ft_hex_dump(&ls_data.args, sizeof(t_args), 16);
	print_list(ls_data.args.cli_files_list);
	ft_printf("List size: %d\n", ft_lstsize(ls_data.args.cli_files_list));
	
	free_allocated_memory(&ls_data);
	return (EXIT_SUCCESS);
}
