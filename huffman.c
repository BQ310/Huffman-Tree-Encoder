#include "huffman.h"
#include "pq.h"
#include "stack.h"
#include "io.h"
#include <unistd.h>
#include <stdio.h>

//creates a huffman tree from a given histogram
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *q = pq_create(2 * ALPHABET);
    PriorityQueue **dq = &q;
    Node *n;

    //create a node for any non zero symbol and enqueues it
    for (int i = 0; i < ALPHABET; i++) {
        if (hist[i] != 0) {
            n = node_create(i, hist[i]);
            enqueue(q, n);
        }
    }

    Node l;
    Node *left = &l;

    Node r;
    Node *right = &r;

    Node ro;
    Node *root = &ro;

    //while the queue has at least 2 elements
    //dequeue the top two the enqueue its created parent
    while (pq_size(q) >= 2) {
        dequeue(q, &left);
        dequeue(q, &right);
        enqueue(q, node_join(left, right));
    }

    //the last remaining node is the root
    dequeue(q, &root);

    pq_delete(dq);

    return root;
}

//builds a code table by traversing the huffman tree
void build(Node *n, Code *table, Code *c) {

    uint8_t bit;

    if (n) {
        //if the node is a leaf then give the table its code
        if (!(n->left) && !(n->right)) {
            //printf("top:%u\n", c->top);
            table[n->symbol] = *c;
        } else {
            // post order traversal to build up code for each leaf node
            code_push_bit(c, 0);
            build(n->left, table, c);
            code_pop_bit(c, &bit);

            code_push_bit(c, 1);
            build(n->right, table, c);
            code_pop_bit(c, &bit);
        }
    }
}

//helper function for building a code table
void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    build(root, table, &c);
}

//dumps a tree to outfile
void dump_tree(int outfile, Node *root) {
    uint8_t buf[2];

    if (root) {
        //post order traversal
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);

        //if the node is a leaf then print L with its symbol
        if (!(root->left) && !(root->right)) {
            buf[0] = 'L';
            buf[1] = root->symbol;

            write_bytes(outfile, buf, 2);
        } else {
            //if it is a intermediary node the print I
            buf[0] = 'I';
            write_bytes(outfile, buf, 1);
        }
    }
}

//recreates a tree from a tree dump
Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
    Stack *s = stack_create(nbytes);

    Node r;
    Node *right = &r;
    Node l;
    Node *left = &l;
    Node ro;
    Node *root = &ro;

    //traverse through the whole tree dump
    for (int i = 0; i < nbytes; i++) {
        //if L then push the symbol next to it onto the stack as a node
        if (tree_dump[i] == 'L') {
            if (stack_push(s, node_create(tree_dump[i + 1], 0))) {
                i++;
            }
            //if intermiate node then pop 2 items off the stack and push their parents
        } else if (tree_dump[i] == 'I') {
            stack_pop(s, &right);
            stack_pop(s, &left);

            stack_push(s, node_join(left, right));
        }
    }
    //the last remaining node is the root
    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}

//deletes a tree through post order traversal
void delete_tree(Node **root) {
    if (*root) {
        Node *l = (*root)->left;
        Node *r = (*root)->right;
        delete_tree(&l);
        delete_tree(&r);
        node_delete(root);
    }
    *root = NULL;
}
