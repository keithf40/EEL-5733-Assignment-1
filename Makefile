CC = gcc
CFLAGS = -Wall -Wextra

all: magic_transformer transformer1 transformer2 transformer3

magic_transformer: magic_transformer.c
	$(CC) $(CFLAGS) -o magic_transformer magic_transformer.c

transformer1: transformer1.c
	$(CC) $(CFLAGS) -o transformer1 transformer1.c

transformer2: transformer2.c
	$(CC) $(CFLAGS) -o transformer2 transformer2.c

transformer3: transformer3.c
	$(CC) $(CFLAGS) -o transformer3 transformer3.c

clean:
	rm -f magic_transformer transformer1 transformer2 transformer3
	