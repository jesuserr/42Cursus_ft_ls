/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:22:47 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/15 00:02:33 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Uses the fast and slow pointer technique to split the list into two halves.
// First of all checks again if the list is empty or has only one element (it
// has already been checked in sort_list() but is good practice to check it).
// Then using the fast and slow pointers, it finds the middle of the list and
// splits it into two halves by setting the next pointer of the slow pointer to
// NULL.
static void	split_list(t_list *head, t_list **middle)
{
	t_list	*fast;
	t_list	*slow;

	if (!head || !head->next)
	{
		*middle = NULL;
		return ;
	}
	slow = head;
	fast = head->next;
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	*middle = slow->next;
	slow->next = NULL;
}

// TODO: Pass comparison function as a parameter to allow for different sorting
// criteria. Right now it only sorts by string comparison of the content of the
// list nodes.
// Function could be recursive but it is implemented as iterative to avoid
// potential stack overflow for very large lists ('ls' from root directory).
// Uses dummy node technique which is a common pattern in linked list
// manipulation to simplify code by eliminating special cases for the head node.
// Tail is used to keep track of the last node in the merged list.
static t_list	*merge_lists(t_list *a, t_list *b)
{
	t_list	dummy;
	t_list	*tail;

	if (!a)
		return (b);
	if (!b)
		return (a);
	dummy.next = NULL;
	tail = &dummy;
	while (a && b)
	{
		if (ft_strcmp((char *)a->content, (char *)b->content) <= 0)
		{
			tail->next = a;
			a = a->next;
		}
		else
		{
			tail->next = b;
			b = b->next;
		}
		tail = tail->next;
	}
	if (a)
		tail->next = a;
	else
		tail->next = b;
	return (dummy.next);
}

void	sort_list(t_list **list)
{
	t_list	*head;
	t_list	*middle;

	if (!list || !*list || !(*list)->next)
		return ;
	head = *list;
	split_list(head, &middle);
	sort_list(&head);
	sort_list(&middle);
	*list = merge_lists(head, middle);
}
