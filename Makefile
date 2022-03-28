CC=gcc								# compilador
CFLAGS=-Wall -O3 -g		# opções de compilação
TARGET=wordboard			# executável

all: wordboard.c
	$(CC) wordboard.c -o ${TARGET}

clean:
	rm *.o ${TARGET}