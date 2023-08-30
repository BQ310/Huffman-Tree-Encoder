# Assignment 6: Huffman Coding

## Description
Compresses and decompresses an input file using a huffman encoding algorithm

## Command Line Arguments
Encode:
	- i the input file to compress
	- o the output file to send compressed message to
	- v prints the compression statistics
	- h prints the usage of the program
Decode:
	- i the compressed file to decompress
	- o the output file to send decompressd message to
	- v prints the decompression statistics
	- h prints the usage of the program

## Running
./encode -i infile -o outfile
Compresses infile and outputs the compression to outfile

./decode -i infile -o outfile
Decompresses infile and outputs decompressed to outfile

## Scan Build
1 scan build warning
decode.c:98:14: warning: Access to field 'left' results in a dereference of a null pointer (loaded from variable 'n')
```        
	if (!(n->left) && !(n->right)) {
             ^~~~~~~~~
```
Not an issue because it is intentionally meant to access a null pointer.

## Valgrind
No Memory leaks
