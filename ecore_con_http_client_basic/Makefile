CC=gcc

all:
	$(CC) -g -o client{,.c} `pkg-config --libs --cflags eina ecore ecore-con`

clean:
	rm client