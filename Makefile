CC=gcc
CFLAGS=-Wall -I.
OBJ = main.o graph.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

escalona: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) escalona
