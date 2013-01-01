STD=c99
OPT_LEVEL=3
CC=gcc -Wall -std=$(STD) -O$(OPT_LEVEL)

deflate: main.c deflate.o lz_element.o lz_queue.o hash_table.o util.o list.o bit_stream.o bit_vec.o
	$(CC) -o deflate $^

deflate.o: deflate.c util.o lz_element.o lz_queue.o list.o hash_table.o bit_stream.o bit_vec.o
	$(CC) -c $<

util.o: util.c
	$(CC) -c $^

lz_element.o: lz_element.c util.o
	$(CC) -c $<

lz_queue.o: lz_queue.c lz_element.o util.o
	$(CC) -c $<

list.o: list.c
	$(CC) -c $^

hash_table.o: hash_table.c list.o
	$(CC) -c $<

bit_vec.o: bit_vec.c util.o
	$(CC) -c $<

bit_stream.o: bit_stream.c bit_vec.o util.o
	$(CC) -c $<

clean:
	rm -rf deflate *.o ../tests/output*