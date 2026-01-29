#include <stddef.h>
#include "libft.h"

int ft_lstsize(t_list *lst) {
    if (lst == 0) {return 0;}
    size_t i = 0;
    while (lst->next != 0){
        lst = lst->next;
        i++;
    }
    return i+1;
}