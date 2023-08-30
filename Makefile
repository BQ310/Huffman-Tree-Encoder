CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra

all: encode decode

encode: encode.c node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o encode encode.c node.o pq.o code.o io.o stack.o huffman.o

decode: decode.c node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o decode decode.c node.o pq.o code.o io.o stack.o huffman.o


node.o: node.c
	$(CC) $(CFLAGS) -c node.c

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c

code.o: code.c
	$(CC) $(CFLAGS) -c code.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[c,h]
