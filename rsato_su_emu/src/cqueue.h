//
// Created by robin on 2/18/22.
//

#include <malloc.h>

#ifndef FAR_STATION_CQUEUE_H
#define FAR_STATION_CQUEUE_H

#endif //FAR_STATION_CQUEUE_H

typedef struct node {
    char *val;
    struct node *next;
} node_t;

void enqueue(node_t **head, char val[]) {
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) return;

    new_node->val = val;
    new_node->next = *head;

    *head = new_node;
}

char* dequeue(node_t **head) {
    node_t *current, *prev = NULL;
    int retval = -1;

    if (*head == NULL) return -1;

    current = *head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    retval = current->val;
 //free(current);

    if (prev)
        prev->next = NULL;
    else
        *head = NULL;

    return retval;
}