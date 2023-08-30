#include "code.h"
#include "defines.h"
#include <stdio.h>

//creates a code with 0's
Code code_init(void) {
    Code c;
    c.top = 0;
    for (uint8_t i = 0; i < MAX_CODE_SIZE; i++) {
        (c.bits)[i] = 0;
    }
    return c;
}

// returns code size
uint32_t code_size(Code *c) {
    return c->top;
}

//if the code is empty then returns true
bool code_empty(Code *c) {
    if (c->top <= 0) {
        return true;
    } else {
        return false;
    }
}

// if the code is full then returns true
bool code_full(Code *c) {
    //c
    if (c->top >= 256) {
        return true;
    } else {
        return false;
    }
}

//sets a bit in the code to 1
bool code_set_bit(Code *c, uint32_t i) {
    if (i >= 256) {
        return false;
    } else {
        uint32_t byte = i / 8;
        uint32_t rbit = i % 8;
        //sets the bit at byte to 1
        (c->bits)[byte] = ((c->bits)[byte] | (1 << rbit));
        return true;
    }
}

//clears a bit in the code to 0
bool code_clr_bit(Code *c, uint32_t i) {
    if (i >= 256) {
        return false;
    } else {
        uint32_t byte = i / 8;
        uint32_t rbit = i % 8;
        //sets the bit in byte to 0
        (c->bits)[byte] = (c->bits)[byte] & ~(1 << rbit);
        return true;
    }
}

//returns the bit value at an index
bool code_get_bit(Code *c, uint32_t i) {
    if (i >= 256) {
        return false;
    } else {
        uint32_t byte = i / 8;
        uint32_t rbit = i % 8;
        //returns the bit at i
        return ((c->bits)[byte] & (1 << rbit)) >> rbit;
    }
}

// pushes the bit onto the code
bool code_push_bit(Code *c, uint8_t bit) {

    bool result;
    //c
    if (code_full(c)) {
        return false;
    }
    if (bit) {
        result = code_set_bit(c, c->top);
    } else {
        result = code_clr_bit(c, c->top);
    }
    c->top = c->top + 1;
    //printf("top:%d\n", c->top);
    return result;
}

//removes the top bit and returns it
bool code_pop_bit(Code *c, uint8_t *bit) {

    if (code_empty(c)) {
        return false;
    }
    if (code_get_bit(c, c->top)) {
        *bit = 1;
    } else {
        *bit = 0;
    }
    c->top = c->top - 1;
    //printf("top:%d\n", c->top);
    return true;
}

// prints the code
void code_print(Code *c) {
    for (uint32_t i = 0; i < c->top; i++) {
        if (code_get_bit(c, i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}
