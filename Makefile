CC=gcc								# compilador
CFLAGS=-Wall -O3 -g		# opções de compilação
TARGET=build					# directório de saída

all: wordboard.c
	mkdir -p ${TARGET}
	cd ${TARGET} && $(CC) $(CFLAGS) ../wordboard.c -o wordboard

clean:
	rm -rf ${TARGET}