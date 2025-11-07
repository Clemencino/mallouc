CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -fvisibility=hidden -fPIC
CPPFLAGS = -D_DEFAULT_SOURCE
LDFLAGS = -shared -Wl,--no-undefined

SRCS = src/malloc.c src/bucket.c
OBJS = ${SRCS:.c=.o}

library: $(OBJS)
	$(CC) $(LDFLAGS) -o libmalloc.so $(OBJS)

src/malloc.o: src/malloc.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o src/malloc.o -c src/malloc.c

src/bucket.o: src/bucket.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o src/bucket.o -c src/bucket.c 

check:
	/tests/test2.sh

clean:
	$(RM) $(OBJS) libmalloc.so
