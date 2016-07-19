all: test
test: test.c thread_pool.c thread_pool.h
	gcc -g -Wall -D_TP_DEBUG test.c thread_pool.c -lpthread -o $@
clean:
	rm -rf test *.o
