#include <stdlib.h>
#include "libft.h"

t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *)) {
    if (lst == 0 || f == 0 || del == 0) {return 0;}
    void *currentContent = f(lst->content);
    t_list *firstNode = ft_lstnew(currentContent);
    if (firstNode == 0) { //dell everything
            del(currentContent);
            return 0;
        }
    
    t_list *currentNode = firstNode;
    lst = lst->next; //processed the first node, so go next

    while(lst != 0) {
        currentContent = f(lst->content);
        currentNode->next = ft_lstnew(currentContent);
        if (currentNode->next == 0) { //dell everything
            del(currentContent);
            ft_lstclear(&firstNode, del);
            return 0;
        }
        currentNode = currentNode->next; //go next
        lst = lst->next; //go next
    }
    return firstNode;
}
