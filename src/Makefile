CC=gcc -Wall -std=c99

lz_test: lz_test.o lz.o lz_stream.o lz_element.o stream.o
	$(CC) -o tests/lz_test $^

clean:
	rm -rf lz.o lz_element.o lz_stream.o lz_test.o stream.o tests/lz_test a.out

stream.o: stream/stream.c
	$(CC) -c $^

lz_element.o: lz/lz_element.c
	$(CC) -c $^

lz_stream.o: lz/lz_stream.c stream.o lz_element.o
	$(CC) -c $<

lz.o: lz/lz.c lz_stream.o 
	$(CC) -c $<

lz_test.o: lz/lz_test.c lz.o
	$(CC) -c $<


