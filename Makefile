# Copyright 2020 Darius Neatu <neatudarius@gmail.com>

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -g
Stack=Stack
LinkedList=LinkedList

.PHONY: build clean

build: rope

rope: rope.o $(Stack).o $(LinkedList).o test6.o
	$(CC) $^ -o $@

rope.o: rope.c
	$(CC) $(CFLAGS) $^ -c

$(Stack).o: $(Stack).c $(Stack).h
	$(CC) $(CFLAGS) $^ -c

$(LinkedList).o: $(LinkedList).c $(LinkedList).h
	$(CC) $(CFLAGS) $^ -c
test6.o: ./tests/test6.c
	$(CC) $(CFLAGS) $^ -c

pack:
	zip -FSr 3XYCA_FirstnameLastname_Tema1.zip README Makefile *.c *.h

clean:
	rm -f *.o rope *.h.gch

.PHONY: pack clean
