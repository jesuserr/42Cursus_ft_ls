/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 21:24:34 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/20 13:10:48 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Auxiliar function for debugging purposes. Delete at the end of the project.
void	print_list_aux(t_list *list)
{
	while (list)
	{
		ft_printf("Content: %s\n", (char *)list->content);
		list = list->next;
	}
}

// Auxiliar function for debugging purposes. Delete at the end of the project.
void	print_list_aux2(t_list *list)
{
	t_list			*backup;
	struct dirent	*subdir_entry;

	backup = list;
	while (backup)
	{
		subdir_entry = &((t_entry_data *)backup->content)->entry;
		ft_printf("- %s\n", subdir_entry->d_name);
		backup = backup->next;
	}
}

void	print_list(t_args *args, t_list *entries_list, const char *current_path)
{
	t_entry_data	*entry_data;

	if (!args->first_printing)
		ft_printf("\n");
	else
		args->first_printing = false;
	if (ft_lstsize(args->cli_dirs_list) > 1)
	{
		if (ft_strchr(current_path, ' ') != NULL)
			ft_printf("'%s':\n", current_path);
		else
			ft_printf("%s:\n", current_path);
	}
	while (entries_list)
	{
		entry_data = (t_entry_data *)entries_list->content;
		if (entry_data->entry.d_name[0] == '.' && !args->all)
		{
			entries_list = entries_list->next;
			continue ;
		}
		if (ft_strchr(entry_data->entry.d_name, ' ') != NULL)
			ft_printf("'%s'  ", entry_data->entry.d_name);
		else
			ft_printf("%s  ", entry_data->entry.d_name);
		entries_list = entries_list->next;
	}
	ft_printf("\n");
}
