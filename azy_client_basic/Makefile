CC=gcc

all:
	$(CC) -g -Wall client.c -o client `pkg-config --libs --cflags eina ecore azy`

clean:
	rm client
