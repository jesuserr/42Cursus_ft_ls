/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:22:47 by jesuserr          #+#    #+#             */
/*   Updated: 2025/07/18 21:09:46 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// Comparison function to sort the list of command line arguments by their name.
// List is contained in args->cli_files_list. In that case *content of the list
// nodes is just a string (file/directory name).
int	compare_names_cli(const void *a, const void *b, bool reverse)
{
	if (reverse)
		return (ft_strcmp((char *)b, (char *)a));
	else
		return (ft_strcmp((char *)a, (char *)b));
}

// Comparison function to sort the list of directory entries by their d_name.
int	compare_d_names(const void *a, const void *b, bool reverse)
{
	t_entry_data	*entry_a;
	t_entry_data	*entry_b;

	entry_a = (t_entry_data *)a;
	entry_b = (t_entry_data *)b;
	if (reverse)
		return (ft_strcmp(entry_b->entry.d_name, entry_a->entry.d_name));
	else
		return (ft_strcmp(entry_a->entry.d_name, entry_b->entry.d_name));
}

// Comparison function to sort the list of directory entries by their st_mtime.
// Time of last modification is used to sort the entries.
// First approach was to subtract the two times and return the result, but
// that approach was not safe because the result could overflow if difference
// between both time was bigger than an int.
// When last modification times are the same (same second), the comparison is
// done by comparing the nanoseconds (st_mtim.tv_nsec) to ensure a stable sort.
// As last resort, if both times and nanoseconds are the same, the names
// are compared to ensure a stable sort.
int	compare_stat_times(const void *a, const void *b, bool reverse)
{
	t_entry_data	*entry_a;
	t_entry_data	*entry_b;
	int8_t			result;

	entry_a = (t_entry_data *)a;
	entry_b = (t_entry_data *)b;
	result = -1;
	if (reverse)
		result = 1;
	if (entry_a->stat_buf.st_mtime > entry_b->stat_buf.st_mtime)
		return (1 * result);
	if (entry_a->stat_buf.st_mtime < entry_b->stat_buf.st_mtime)
		return (-1 * result);
	if (entry_a->stat_buf.st_mtim.tv_nsec > entry_b->stat_buf.st_mtim.tv_nsec)
		return (1 * result);
	if (entry_a->stat_buf.st_mtim.tv_nsec < entry_b->stat_buf.st_mtim.tv_nsec)
		return (-1 * result);
	return (ft_strcmp(entry_b->entry.d_name, entry_a->entry.d_name));
}

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

// Comparison function 'compare' is passed as a parameter to allow for different
// sorting criteria (name, time, etc.).
// Function could be recursive but it is implemented as iterative to avoid
// potential stack overflow for very large lists ('ls' from root directory).
// Uses dummy node technique which is a common pattern in linked list
// manipulation to simplify code by eliminating special cases for the head node.
// Tail is used to keep track of the last node in the merged list.
static t_list	*merge_lists(t_list *a, t_list *b, t_compare_func compare, bool reverse)
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
		if (compare(a->content, b->content, reverse) <= 0)
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

// Sorts a linked list using the merge sort algorithm.
// https://www.youtube.com/watch?v=TGveA1oFhrc
// If the list is empty or has only one element, it returns since it is sorted.
// The sort_list() function takes a double pointer (t_list **list) instead of a
// single pointer (t_list *list) because it needs to modify the original pointer
// that points to the head of the list. Also takes as a parameter a function
// pointer 'compare' to allow for different sorting criteria (name, time, etc.)
void	sort_list(t_list **list, t_compare_func compare, bool reverse)
{
	t_list	*head;
	t_list	*middle;

	if (!list || !*list || !(*list)->next)
		return ;
	head = *list;
	split_list(head, &middle);
	sort_list(&head, compare, reverse);
	sort_list(&middle, compare, reverse);
	*list = merge_lists(head, middle, compare, reverse);
}
