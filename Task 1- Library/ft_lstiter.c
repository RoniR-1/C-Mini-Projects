#include "libft.h"

void ft_lstiter(t_list *lst, void (*f)(void *)) {
    if (lst == 0 || f == 0) {return;}
    t_list *next;
    while (lst != 0) {
        next = lst->next;
        f(lst->content);
        lst = next;
    }
}