#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

//create a node with symbol and frequency
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(1 * sizeof(Node));
    n->symbol = symbol;
    n->frequency = frequency;
    n->left = NULL;
    n->right = NULL;
    return n;
}

//delete a node and null the pointer
void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}

//create a parent node using the values of two children
Node *node_join(Node *left, Node *right) {
    Node *parent = (Node *) malloc(1 * sizeof(Node));
    parent->symbol = '$';
    parent->frequency = left->frequency + right->frequency;
    parent->left = left;
    parent->right = right;
    return parent;
}

//prints a node and its properties
void node_print(Node *n) {
    printf("Symbol:%c, Frequency: %" PRIu64 "\n", n->symbol, n->frequency);
}
