CC=gcc

EXEC=testproc read_image buildHistogram distance
NOMSRCS=cgiu.c proc.c rdjpeg.c read_image.c testproc.c 
SRCSFOLDER=sources
HEADERFOLDER=headers
OBJFOLDER=build
SRCS=$(addprefix $(SRCSFOLDER)/, $(NOMSRCS))
OBJS=$(SRCS:.c=.o)
CFLAGS=-Wall -I./headers/
all:$(EXEC)


# Object rules
$(OBJFOLDER)/histIO.o: $(SRCSFOLDER)/histIO.c $(HEADERFOLDER)/histIO.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJFOLDER)/rdjpeg.o: $(SRCSFOLDER)/rdjpeg.c $(HEADERFOLDER)/rdjpeg.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJFOLDER)/proc.o: $(SRCSFOLDER)/proc.c $(HEADERFOLDER)/proc.h
	$(CC) -o $@ -c $< $(CFLAGS)


# Executables rules

read_image:$(SRCSFOLDER)/read_image.c $(SRCSFOLDER)/rdjpeg.c
	$(CC) -o $@ $^ $(CFLAGS)

testproc:$(SRCSFOLDER)/proc.c $(SRCSFOLDER)/testproc.c $(SRCSFOLDER)/cgiu.c
	$(CC) -o $@ $^ $(CFLAGS)


buildHistogram:$(SRCSFOLDER)/buildHistogram.c $(SRCSFOLDER)/rdjpeg.c $(OBJFOLDER)/histIO.o
	$(CC) -o $@ $^ $(CFLAGS)


distance:$(SRCSFOLDER)/distance.c $(OBJFOLDER)/histIO.o $(OBJFOLDER)/rdjpeg.o $(OBJFOLDER)/proc.o 
	$(CC) -o $@ $^ $(CFLAGS)



# Misc rules
clean:
	rm -rf $(OBJFOLDER)/* $(EXEC)