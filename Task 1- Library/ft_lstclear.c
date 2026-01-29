#include <stdlib.h>
#include "libft.h"

void ft_lstclear(t_list **lst, void (*del)(void*)) {
    t_list *next;
    if (lst == 0 || *lst == 0 || del == 0)
        return;
    t_list *current = *lst;
    while (current != 0) {
        next = current->next;
        ft_lstdelone(current, del);
        current = next;
    }
    *lst = 0;
}
