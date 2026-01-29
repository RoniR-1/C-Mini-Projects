#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

void ft_lstadd_front(t_list **lst, t_list *new) {
    new->next = *lst;
    *lst = new;
    return;
}

int ft_lstsize(t_list *lst) {
    if (lst == 0) {return 0;}
    size_t i = 0;
    while (lst->next != 0){
        lst = lst->next;
        i++;
    }
    return i + 1;
}

#include "libft.h"

t_list *ft_lstlast(t_list *lst) {
    if (lst == 0) {return 0;}
    while (lst->next != 0) {
        lst = lst->next;
    }
    return lst;
}

int main() {    
    // Test 1: Empty list
    t_list *list = NULL;
    printf("Test 1 - Empty list:\n");
    printf("List size before: %d\n", ft_lstsize(list));
    printf("List address: %p\n\n", (void *)list);
    
    // Test 2: Add first node
    printf("Test 2 - Adding first node:\n");
    t_list *node1 = malloc(sizeof(t_list));
    node1->content = "First node";
    node1->next = NULL;
    ft_lstadd_front(&list, node1);
    printf("Added node with content: %s\n", (char *)node1->content);
    printf("List size after first add: %d\n", ft_lstsize(list));
    printf("List head content: %s\n", (char *)list->content);
    printf("List head next: %p\n\n", (void *)list->next);
    
    // Test 3: Add second node
    printf("Test 3 - Adding second node:\n");
    t_list *node2 = malloc(sizeof(t_list));
    node2->content = "Second node";
    node2->next = NULL;
    ft_lstadd_front(&list, node2);
    printf("Added node with content: %s\n", (char *)node2->content);
    printf("List size after second add: %d\n", ft_lstsize(list));
    printf("List head content: %s\n", (char *)list->content);
    printf("List head next points to: %p\n", (void *)list->next);
    printf("Second node content via next: %s\n\n", (char *)list->next->content);
    
    // Test 4: Add third node
    printf("Test 4 - Adding third node:\n");
    t_list *node3 = malloc(sizeof(t_list));
    node3->content = "Third node";
    node3->next = NULL;
    ft_lstadd_front(&list, node3);
    printf("Added node with content: %s\n", (char *)node3->content);
    printf("List size after third add: %d\n", ft_lstsize(list));
    
    // Traverse and print all nodes
    printf("\nTraversing list:\n");
    t_list *current = list;
    int i = 1;
    while (current != NULL) {
        printf("  Node %d: %s (next: %p)\n", 
               i, 
               (char *)current->content, 
               (void *)current->next);
        current = current->next;
        i++;
    }
    
    printf("last node: %p", ft_lstlast(list));

    // Cleanup
    while (list != NULL) {
        t_list *temp = list->next;
        free(list);
        list = temp;
    }
    printf("\nAll tests completed.\n");
    return 0;
}