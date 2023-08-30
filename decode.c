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

//usage function prints command line options and uses
void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "Decoded a compressed message using a huffman tree\n"
        "\n"
        "USAGE\n"
        " %s [hi:o:v] [-i infile] [-o outfile]\n"
        "\n"
        "OPTIONS\n"
        " -h prints usage of program\n"
        " -i file to decompress\n"
        " -o file to send decompressed message to \n"
        " -v prints decompression statistics\n",
        exec);
}

//prints each node of a tree in post order fashion
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

    //getopt loop to get command arguments
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in = optarg; break;
        case 'o': out = optarg; break;
        case 'v': v = true; break;
        case 'h': usage(argv[0]); break;
        default: return EXIT_FAILURE;
        }
    }

    int infile;
    int outfile;

    //use stdin and stdout as the defaults
    if (in != NULL) {
        infile = open(in, O_RDONLY);
        if (infile == -1) {
            printf("infile failed to open\n");
        }
    } else {
        infile = 0;
    }
    if (out != NULL) {
        outfile = open(out, O_CREAT | O_WRONLY);
        if (outfile == -1) {
            printf("outfile failed to open\n");
        }
    } else {
        outfile = 1;
    }

    //read the header
    Header h;
    read_bytes(infile, (uint8_t *) &h, sizeof(Header));

    //check if it is a compressed file
    if (h.magic != MAGIC) {
        printf("error file passed: not compressed\n");
        return EXIT_FAILURE;
    }

    //change the permissions of outfile to the same as infile
    fchmod(outfile, h.permissions);

    //read in the tree
    uint8_t buf[h.tree_size];
    read_bytes(infile, buf, h.tree_size);

    //rebuilds the tree without frequency
    Node *root = rebuild_tree(h.tree_size, buf);
    Node *n = root;

    uint32_t decoded = 0;
    uint8_t bit;

    //if the bit is 1 go right and if its 0 then go left
    //when a leaf is reached print the leaf's symbol and then start again from root
    while (decoded < h.file_size) {
        if (!(n->left) && !(n->right)) {
            write_bytes(outfile, &(n->symbol), 1);

            n = root;
            decoded++;
        }
        if (read_bit(infile, &bit)) {
            n = n->right;
        } else {
            n = n->left;
        }
    }

    //decompression statistics
    if (v) {
        struct stat ho;
        fstat(outfile, &ho);
        struct stat hi;
        fstat(infile, &hi);
        double compressed = hi.st_size;
        double decompressed = ho.st_size;
        double space_saved = 100 * (1 - (compressed / decompressed));
        fprintf(stderr,
            "compressed: %f\n"
            "decompressed: %f\n"
            "space saved: %f\n",
            compressed, decompressed, space_saved);
    }

    delete_tree(&root);
    close(infile);
    close(outfile);
}
