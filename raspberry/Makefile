LIBS=-lpthread

CCOPT=-std=c99 -D_DEFAULT_SOURCE -g

all: mis24

mis24:	mis24.c comm.c
	gcc -o mis24 $^ $(LIBS) $(CCOPT)

clean:
	rm -f mis24
