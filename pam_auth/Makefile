CC=gcc
LDFLAGS=-lpam
OBJ = main.o

%.o: %.c
	$(CC) -c -o $@ $<

main: $(OBJ)
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o main