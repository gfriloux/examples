CC=gcc

all:
	$(CC) -g main.c -o main `pkg-config --libs --cflags eina ecore ecore-con`

debug:
	$(CC) -g -finstrument-functions main.c ../ptrace.c -o main `pkg-config --libs --cflags eina ecore ecore-con`


clean:
	rm -f main
