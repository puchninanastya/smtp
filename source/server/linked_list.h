#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct  {
    int data;
    struct node *next;
} node;

node* linked_list_add_node( node *head, int data_to_add );
node* linked_list_delete_node( node *head, int data_to_delete );

#endif