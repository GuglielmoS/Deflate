CC=gcc -Wall -std=c99

deflate: main.c deflate.o lz_element.o lz_queue.o hash_table.o util.o list.o
	$(CC) -o deflate $^

deflate.o: deflate.c util.o lz_element.o lz_queue.o list.o hash_table.o
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

clean:
	rm -rf deflate *.o ../tests/output*