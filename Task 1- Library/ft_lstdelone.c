#include <stdlib.h>
#include "libft.h"

void ft_lstdelone(t_list *lst, void (*del)(void *)) {
    if (lst == 0 || del == 0) {return;}
    del(lst->content);
    free(lst);
    return;
}