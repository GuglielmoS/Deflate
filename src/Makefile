CC=gcc -Wall -std=c99

tests: lz_test huffman_test history_buffer_test list_test hash_table_test deflate_test file_stream_test

clean:
	rm -rf *.o tests/*_test

# GENERAL

stream.o: stream/stream.c
	$(CC) -c $^

util.o: util.c
	$(CC) -c $^

# FILE STREAM
file_stream_test: file_stream_test.o file_stream.o
	$(CC) -o tests/file_stream_test $^

file_stream.o: file_stream/file_stream.c
	$(CC) -c $^

file_stream_test.o: file_stream/file_stream_test.c file_stream.o
	$(CC) -c $<

# HISTORY BUFFER 

history_buffer_test: history_buffer_test.o history_buffer.o
	$(CC) -o tests/history_buffer_test $^

history_buffer.o: history_buffer/history_buffer.c
	$(CC) -c $^

history_buffer_test.o: history_buffer/history_buffer_test.c history_buffer.o
	$(CC) -c $<

# LIST

list_test: list_test.o list.o
	$(CC) -o tests/list_test $^

list.o: list/list.c
	$(CC) -c $^

list_test.o: list/list_test.c list.o
	$(CC) -c $<


# HASH_TABLE

hash_table_test: hash_table_test.o hash_table.o list.o
	$(CC) -o tests/hash_table_test $^

hash_table.o: hash_table/hash_table.c
	$(CC) -c $^

hash_table_test.o: hash_table/hash_table_test.c hash_table.o list.o
	$(CC) -c $<

# DEFLATE

deflate_test: deflate_test.o deflate.o list.o hash_table.o file_stream.o history_buffer.o file_stream.o
	$(CC) -o tests/deflate_test $^

deflate.o: deflate.c
	$(CC) -c $^

deflate_test.o: deflate_test.c deflate.o 
	$(CC) -c $<

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

# LZ_QUEUE

lz_queue.o: lz/lz_queue.c
	$(CC) -c $^

# HUFFMAN

huffman_test: huffman.o huffman_test.o util.o
	$(CC) -o tests/huffman_test $^

huffman_test.o: huffman/huffman_test.c huffman.o
	$(CC) -c $<

huffman.o: huffman/huffman.c
	$(CC) -c $<



