#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

//defines a stack structure
struct Stack {
    Node **items;
    uint32_t top;
    uint32_t capacity;
};

//creates a stack on the heap with a capacity
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    Node **items = (Node **) malloc(sizeof(Node *) * capacity);
    uint32_t top = 0;
    s->items = items;
    s->top = top;
    s->capacity = capacity;
    return s;
}

//frees a stack from heap and nulls pointer
void stack_delete(Stack **s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
}

//checks if the stack is empty
bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    } else {
        return false;
    }
}

//checks if the stack is full
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    } else {
        return false;
    }
}

//returns the size of the stack
uint32_t stack_size(Stack *s) {
    return s->top;
}

//adds node onto the top of the stack
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    } else {
        (s->items)[s->top] = n;
        s->top = s->top + 1;
        return true;
    }
}

//removes and returns node from the top of the stack
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    } else {
        Node **items = s->items;
        *n = items[s->top - 1];
        s->top = s->top - 1;
        return true;
    }
}

//prints the stack
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->top; i++) {
        printf("index:%d, ", i);
        node_print((s->items)[i]);
    }
}
