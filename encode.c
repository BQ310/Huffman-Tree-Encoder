#include "huffman.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "io.h"
#include "header.h"

#define OPTIONS "hi:o:v"

//prints the usage of the program and command line arguments
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "Encodes a message using a huffman encoding algorithm\n"
        "\n"
        "USAGE\n"
        " %s [hi:o:v] [-i infile] [-o outfile]\n"
        "\n"
        "OPTIONS\n"
        " -h prints usage of program\n"
        " -i file to encode\n"
        " -o file to send encoding to\n"
        " -v prints compression statistics\n",
        exec);
}

//prints a tree in post order
void print_tree(Node *root) {
    if (root) {
        print_tree(root->left);
        print_tree(root->right);
        node_print(root);
    }
}

int main(int argc, char **argv) {
    char *in = NULL;
    char *out = NULL;
    bool v = false;
    int opt;

    //getopt loop to parse command line arguments
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in = optarg; break;
        case 'o': out = optarg; break;
        case 'v': v = true; break;
        case 'h':
            usage(argv[0]);
            return 0;
            break;
        default: return EXIT_FAILURE;
        }
    }

    int infile;
    int outfile;

    //default use of stdin and stdout for infile and outfile
    if (in != NULL) {
        infile = open(in, O_RDONLY);
        if (infile == -1) {
            printf("infile does not exist\n");
            return 0;
        }
    } else {
        infile = 0;
    }

    if (out != NULL) {
        outfile = open(out, O_CREAT | O_WRONLY);
        if (outfile == -1) {
            printf("outfile does not exist\n");
            return 0;
        }
    } else {
        outfile = 1;
    }

    // create a histogram for each symbol
    uint64_t *hist = (uint64_t *) calloc(ALPHABET, sizeof(uint64_t));

    hist[0] = 1;
    hist[255] = 1;

    uint8_t symbol[BLOCK];

    //read in bytes from infile and increase frequency of read symbol
    while (read_bytes(infile, symbol, BLOCK) > 0) {
        for (int i = 0; i < BLOCK; i++) {
            hist[symbol[i]]++;
        }
    }

    //get the number of unique symbols to calculate tree size
    uint16_t unique = 0;
    for (int i = 0; i < ALPHABET; i++) {
        if (hist[i] != 0) {
            unique++;
        }
    }

    //build the tree
    Node *root = build_tree(hist);

    //print_tree(root);

    //build the codes table
    Code table[ALPHABET];
    build_codes(root, table);

    struct stat hed;
    fstat(infile, &hed);

    //create a header with properties and then export to outfile
    Header h;
    h.magic = MAGIC;
    h.permissions = hed.st_mode;
    h.tree_size = (3 * unique) - 1;
    h.file_size = hed.st_size;

    chmod(out, h.permissions);

    uint8_t *hp = (uint8_t *) &h;
    write_bytes(outfile, hp, sizeof(Header));

    dump_tree(outfile, root);

    lseek(infile, 0, SEEK_SET);

    uint8_t syms[BLOCK] = { 0 };
    int read = 0;
    while ((read = read_bytes(infile, syms, BLOCK)) > 0) {
        for (int i = 0; i < read; i++) {
            write_code(outfile, &table[syms[i]]);
        }
    }
    flush_codes(outfile);

    //data compressions statistics
    if (v) {
        struct stat ho;
        fstat(outfile, &ho);
        double uncompressed = hed.st_size;
        double compressed = ho.st_size;
        double space_saving = 100 * (1 - (compressed / uncompressed));
        fprintf(stderr,
            "uncompressed: %f\n"
            "compressed: %f\n"
            "space saved: %f\n",
            uncompressed, compressed, space_saving);
    }
    delete_tree(&root);
    free(hist);
    close(infile);
    close(outfile);
    return 0;
}
