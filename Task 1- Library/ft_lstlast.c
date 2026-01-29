#include "libft.h"

t_list *ft_lstlast(t_list *lst) {
    if (lst == 0) {return 0;}
    while (lst->next != 0) {
        lst = lst->next;
    }
    return lst;
}