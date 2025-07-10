/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:07:33 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/10 22:01:18 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Frees the memory allocated during program execution.
void	free_allocated_memory(t_ls_data *ls_data)
{
	(void)ls_data;
	return ;
}

int	main(int argc, char **argv)
{
	t_ls_data	ls_data;

	(void)argc;
	ft_bzero(&ls_data, sizeof(t_ls_data));
	parse_arguments(argv, &ls_data);
	ft_hex_dump(&ls_data.args, sizeof(t_args), 16);
	free_allocated_memory(&ls_data);
	return (EXIT_SUCCESS);
}
