//
// Created by Tre on 4/14/2024.
//

/*
 * linked_list.c
 *
 * Implementation of generic doubly linked list
 * (for use by variable hashmap for separate chaining)
 */

#include "linked_list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static Node* new_node(void* data);
static Node* get_node(List* list, int index);
static bool list_is_empty(List* list);

/**
 * Create new linked list node from data
 * @param data
 * @return new linked list node
 */
static Node* new_node(void* data) {
    Node* n = (Node*) malloc(sizeof(Node));
    n->data = data;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

/**
 * Get node from a list at index
 * @param list
 * @param index
 * @return list node at index or NULL if DNE or out of bounds
 */
static Node* get_node(List* list, int index) {
    if (index < 0 || index >= list->size) {
        // printf("Error: cannot get node, list index %d out of bounds.\n", index);
        return NULL;
    } else {
        Node* current = list->head;
        int hop = 0;
        while (hop < index) {
            hop++;
            current = current->next;
        }
        return current;
    }
}

/**
 * Check if list is empty
 * @param list
 * @return 1 for empty, 0 for not empty
 */
static bool list_is_empty(List* list) {
    if (list->head == NULL) {
        return true;
    } else {
        return false;
    }
}

/**
 * Instantiate new linked list
 * @return new List
 */
List* new_list() {
    List* l = (List*) malloc(sizeof(List));
    l->size = 0;
    l->head = NULL;
    return l;
}

/**
 * Insert an item at the end of a list
 * @param list
 * @param data
 */
void list_append(List* list, void* data) {
    Node* node = new_node(data);
    if (!list_is_empty(list)) {
        Node* tail = get_node(list, list->size - 1);
        tail->next = node;
        node->prev = tail;
    } else {
        list->head = node;
        node->prev = list->head;
    }
    list->size++;
}

/**
 * Get data at list index
 * @param list
 * @param index
 * @return data at list index
 */
void* list_get(List* list, int index) {
    if (!list_is_empty(list)) {
        Node* node = get_node(list, index);
        if (node != NULL) {
            void* data = node->data;
            return data;
        } else {
            printf("Error: cannot get data at index %d, none found.\n", index);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Error: Cannot get at index %d, list empty.\n", index);
        exit(EXIT_FAILURE);
    }
}

/**
 * Remove item (node) at list index
 * @param list
 * @param index
 */
void list_remove(List* list, int index) {
    if (!list_is_empty(list)) {
        Node* node_to_remove = get_node(list, index);
        if (node_to_remove != NULL) {
            if (index == 0) { // remove first
                Node* after = node_to_remove->next;
                list->head = after;
                after->prev = list->head;
            } else if (index == (list->size - 1)) { // remove last
                Node* before = node_to_remove->prev;
                before->next = NULL;
            } else { // remove from middle
                Node* before = node_to_remove->prev;
                Node* after = node_to_remove->next;
                before->next = after;
                after->prev = before;
            }

            free(node_to_remove);
            list->size--;
        } else {
            printf("Error: Cannot remove at index %d, index out of bounds.\n", index);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Error: Cannot remove at index %d, empty list.\n", index);
        exit(EXIT_FAILURE);
    }
}

/**
 * Free list from memory
 * @param list
 */
void list_free(List* list) {
    Node* current_node = list->head;
    while (current_node != NULL) {
        Node* next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(list);
}