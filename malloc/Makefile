CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -fvisibility=hidden -fPIC
CPPFLAGS = -D_DEFAULT_SOURCE
LDFLAGS = -shared -Wl,--no-undefined

SRCS = src/malloc.c
OBJS = ${SRCS:.c=.o}

malloc.o: $(SRCS)
	$(CC) -o malloc.o $(SRCS)

library:malloc.o
	ar rcs libmalloc.so malloc.o

check:
	./tests/test.sh

clean:
	$(RM) $(OBJS) $(TARGET) malloc.o libmalloc.so
