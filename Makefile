CC=gcc

EXEC=main
NOMSRCS=compute.c fonctions_simples.c main.c
SRCS=$(addprefix sources/, $(NOMSRCS))
OBJS=$(SRCS:.c=.o)
CFLAGS=-Wall -I./headers/

all:$(EXEC)

main:$(OBJS)
	$(CC) -o $@ $^

#%.o: %.c
#	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf sources/*.o $(EXEC)