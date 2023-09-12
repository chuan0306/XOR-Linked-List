# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>

# define LIST_SRART 0
# define LIST_END 5

typedef struct 
{
    int data;
    intptr_t link;  // pointer-sized integer
}xorlist_t;

// traversing and printing the element of an XOR-linked list
void dump_list(xorlist_t *pt){
    intptr_t prev = (intptr_t) NULL;
    /*  1. used to initialize the variable "prev" (the datatype of "prev" is intptr_t)
        2. set to NULL, because there is no previous node when starting traversal       */
    while (pt)
    {
        printf("%d\n", pt->data);
        xorlist_t * current = pt;
        /*  1. used to remember the current node
            2. for updating the "prev" pointer correctly in next iteration    */
        pt = (xorlist_t *) (pt->link ^ prev);
        /*  1. update "pt" pointer to next node
            2. decode the XORed pointer to obtain the actual address of the nextnode in the list    */
        prev = (intptr_t) current;
        /*  1. used to hold the address of the current node
            2. will be used in the next iteration to decode the next node's address    */
    }
}

// insert head
void insert_head(xorlist_t **head, int data){
    xorlist_t * new_node = malloc(sizeof(xorlist_t));
    new_node-> data = data;

    if (!head){
        new_node -> link = (intptr_t) NULL;
        /*  1. there is no other node in list initially
            2. so the new_node is the only one node in the list, and the link(pre^next) is NULL     */
    } else {
        (*head)->link = (*head)->link ^ (intptr_t) new_node;      
        new_node->link = (intptr_t) *head ^ (intptr_t) NULL;        
    };
}

// remove a node from head
void remove_head(xorlist_t **head){
    /*  double pointer is used because the function needs to modify the 'head' pointer itself
        (to update it when the first node is removed)    */
    
    if(!(*head))        // check whether the linked list is NULL
        return;
    xorlist_t *tmp = (xorlist_t *) (*head)->link;
    // update the 'link' of new head
    
    if (tmp)
        tmp->link ^= (intptr_t) *head;
    /*  1. link of new head(tmp) = (link of new head) ^ (memory address of old head(head))
        2. ensuring the new head can correctly calculate the address of the next node after it    */

    free (*head);
    *head = tmp;    
}

void release_list (xorlist_t *pt){
    intptr_t prev = (intptr_t) NULL;
    while (pt)
    {
        xorlist_t *current = pt;
        pt = (xorlist_t *) (pt->link ^ prev);
        prev = (intptr_t) current;
        free (current);
    }
}

int main(){
    xorlist_t *head = malloc(sizeof(xorlist_t)), *tail;
    xorlist_t *pt = head;
    intptr_t last_node = (intptr_t) NULL;
    for (int c=0; c<LIST_END; ++c){
        xorlist_t *new_node = malloc(sizeof(xorlist_t));
        *pt = (xorlist_t){.data = c, .link=(intptr_t) new_node^last_node};
        last_node = (intptr_t) pt;
        pt = new_node;
    }
    *pt = (xorlist_t){.data = LIST_END, .link = last_node^(intptr_t) NULL};
    tail = pt;

    insert_head(&head, 99);
    dump_list(head);
    remove_head(&tail);
    dump_list(tail);

    release_list(head);

    return 0;
}





