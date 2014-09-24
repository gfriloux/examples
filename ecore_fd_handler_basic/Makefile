CC=gcc

all:
	$(CC) -g main.c -o main `pkg-config --libs --cflags eina ecore`

clean:
	rm -f main
