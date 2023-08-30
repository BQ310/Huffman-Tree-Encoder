#include "io.h"
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>

//static variables for functions
uint64_t bytes_read;
uint64_t bytes_written;

static int windex = 0;
static uint8_t wbuf[BLOCK];

static uint8_t rbuf[BLOCK];
static int rindex = 0;

//reads in bytes using read
//makes sure that read does specified bytes
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    bytes_read = 0;
    int red = 0;
    //if the bytes read is less than nbytes and there is still bytes left to read then loop
    while ((int) bytes_read < nbytes && (red = read(infile, buf + bytes_read, nbytes)) > 0) {
        bytes_read += red;
    }
    return bytes_read;
}

//writes bytes to a outfile
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int writ;
    bytes_written = 0;
    //if the bytes written is less than specified and there are still bytes then loop
    while (
        (int) bytes_written < nbytes && (writ = write(outfile, buf + bytes_written, nbytes)) > 0) {
        bytes_written += writ;
    }
    return bytes_written;
}

//reads bytes into a buffer and then returns bit by bit
bool read_bit(int infile, uint8_t *bit) {

    static int bytes = 0;

    //if a block is done being read get another one
    //also reset the index to 0
    if ((rindex / 8) == bytes) {
        bytes = read_bytes(infile, rbuf, BLOCK);
        rindex = 0;
    }

    int nbyte = rindex / 8;
    int nbit = rindex % 8;

    //get the specified bit from the byte
    *bit = (rbuf[nbyte] & (1 << nbit)) >> nbit;

    rindex++;

    if (*bit <= 0) {
        return false;
    } else {
        return true;
    }
}

//writes a code out to outfile by slowly filling up the bits of a buffer
void write_code(int outfile, Code *c) {

    for (uint32_t i = 0; i < c->top; i++) {
        if (code_get_bit(c, i)) {
            wbuf[windex / 8] = wbuf[windex / 8] | (1 << (windex % 8));
        } else {
            wbuf[windex / 8] = wbuf[windex / 8] & ~(1 << (windex % 8));
        }
        windex++;

        if ((windex / 8) == BLOCK) {
            write_bytes(outfile, wbuf, BLOCK);
            windex = 0;
        }
    }
}

//zero out uneeded bits and then write them out
void flush_codes(int outfile) {
    //zero out the rest of the bits for the last byte
    while ((windex % 8) != 0) {
        wbuf[windex / 8] = wbuf[windex / 8] & ~(1 << (windex % 8));
        windex++;
    }
    write_bytes(outfile, wbuf, windex / 8);
}
