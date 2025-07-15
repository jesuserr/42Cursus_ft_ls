#include "../../libft/includes/ft_printf.h"
#include "../../libft/includes/libft.h"

typedef struct s_content
{
	int				value;
    char			*name;    
}					t_content;

int main(void)
{
    t_content content1 = {42, "Content 1"};
    t_content content2 = {84, "Content 2"};
    t_content content3 = {126, "Content 3"};
    t_list  *my_list;
    t_list  *head;
    
    my_list = ft_lstnew(&content1);
    head = my_list;
    ft_printf("List size: %d\n", ft_lstsize(my_list));
    ft_lstadd_back(&my_list, ft_lstnew(&content2));
    ft_printf("List size: %d\n", ft_lstsize(my_list));
    ft_lstadd_back(&my_list, ft_lstnew(&content3));
    ft_printf("List size: %d\n", ft_lstsize(my_list));
    while (my_list)
    {
        t_content *content = (t_content *)my_list->content;
        ft_printf("Value: %d, Name: %s\n", content->value, content->name);
        my_list = my_list->next;
    }
    my_list = head; // Reset my_list to head for cleanup
    ft_lstclear(&my_list, free);

    return 0;
}
