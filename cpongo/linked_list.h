//
// Created by Tre on 4/14/2024.
//

#ifndef CPONGO_LINKED_LIST_H
#define CPONGO_LINKED_LIST_H

// linked list node type
typedef struct node {
    void* data; // (generic)
    struct node* next;
    struct node* prev;
} Node;

// linked list type
typedef struct list {
    int size;
    Node* head;
} List;

List* new_list();
void list_append(List* list, void* data);
void list_remove(List* list, int index);
void* list_get(List* list, int index);
void list_free(List* list);

#endif //CPONGO_LINKED_LIST_H
