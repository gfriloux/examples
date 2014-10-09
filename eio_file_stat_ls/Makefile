CC=gcc

all:
	$(CC) -g main.c -o main `pkg-config --libs --cflags eina ecore eio`

debug:
	$(CC) -g -finstrument-functions main.c ../ptrace.c -o main `pkg-config --libs --cflags eina ecore eio`


clean:
	rm -f main
