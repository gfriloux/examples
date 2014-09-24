CC=gcc

all:
	$(CC) -g client.c - o client `pkg-config --libs --cflags eina ecore ecore-con`

clean:
	rm client
