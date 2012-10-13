CC=gcc -Wall -std=c99

tests: lz_test huffman_test  

clean:
	rm -rf *.o tests/*

# GENERAL

stream.o: stream/stream.c
	$(CC) -c $^

util.o: util.c
	$(CC) -c $^

# LZ

lz_test: lz_test.o lz.o lz_stream.o lz_element.o stream.o
	$(CC) -o tests/lz_test $^

lz_element.o: lz/lz_element.c
	$(CC) -c $^

lz_stream.o: lz/lz_stream.c stream.o lz_element.o
	$(CC) -c $<

lz.o: lz/lz.c lz_stream.o 
	$(CC) -c $<

lz_test.o: lz/lz_test.c lz.o
	$(CC) -c $<

# HUFFMAN

huffman_test: huffman.o huffman_test.o util.o
	$(CC) -o tests/huffman_test $^

huffman_test.o: huffman/huffman_test.c huffman.o
	$(CC) -c $<

huffman.o: huffman/huffman.c
	$(CC) -c $<



