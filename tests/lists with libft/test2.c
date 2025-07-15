#include "../../libft/includes/ft_printf.h"
#include "../../libft/includes/libft.h"
#include <stdlib.h>

typedef struct s_content
{
    int				value;
    char			*name;    
}					t_content;

int main(void)
{
    t_content *content1 = malloc(sizeof(t_content));
    t_content *content2 = malloc(sizeof(t_content));
    t_content *content3 = malloc(sizeof(t_content));
    t_list  *my_list;
    t_list  *head;
    
    if (!content1 || !content2 || !content3)
        return 1;
        
    content1->value = 42;
    content1->name = "Content 1";
    content2->value = 84;
    content2->name = "Content 2";
    content3->value = 126;
    content3->name = "Content 3";
    
    my_list = ft_lstnew(content1);
    head = my_list;
    ft_printf("List size: %d\n", ft_lstsize(my_list));
    ft_lstadd_back(&my_list, ft_lstnew(content2));
    ft_printf("List size: %d\n", ft_lstsize(my_list));
    ft_lstadd_back(&my_list, ft_lstnew(content3));
    ft_printf("List size: %d\n", ft_lstsize(my_list));
    while (my_list)
    {
        t_content *content = (t_content *)my_list->content;
        ft_printf("Value: %d, Name: %s\n", content->value, content->name);
        my_list = my_list->next;
    }
    my_list = head; // Reset my_list to head for cleanup
    
    // Now it's safe to use free
    ft_lstclear(&my_list, free);

    return 0;
}