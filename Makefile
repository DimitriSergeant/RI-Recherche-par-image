CC=gcc

EXEC=testproc read_image
NOMSRCS=cgiu.c proc.c rdjpeg.c read_image.c testproc.c
SRCSFOLDER=sources
SRCS=$(addprefix $(SRCSFOLDER)/, $(NOMSRCS))
OBJS=$(SRCS:.c=.o)
CFLAGS=-Wall -I./headers/
all:$(EXEC)

read_image:$(SRCSFOLDER)/read_image.c $(SRCSFOLDER)/rdjpeg.c
	$(CC) -o $@ $^ $(CFLAGS)

testproc:$(SRCSFOLDER)/proc.c $(SRCSFOLDER)/testproc.c $(SRCSFOLDER)/cgiu.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf sources/*.o $(EXEC)