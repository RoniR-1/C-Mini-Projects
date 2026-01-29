#include "libft.h"

void ft_lstadd_back(t_list **lst, t_list *new) {
    if (new == 0) {return;}
    if (lst == 0) {
        *lst = new;
    }
    t_list *lastNode = ft_lstlast(*lst);
    lastNode->next = new;
}