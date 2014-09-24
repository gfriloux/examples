CC=gcc

all:
	$(CC) client.c -o client `pkg-config --libs --cflags eina ecore azy`

clean:
	rm client
