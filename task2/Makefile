CC = gcc
CFLAGS = -Wall

all: init destroy check set

init: init.c
    $(CC) $(CFLAGS) init.c -o init

destroy: destroy.c
    $(CC) $(CFLAGS) destroy.c -o destroy

check: check.c
    $(CC) $(CFLAGS) check.c -o check

set: set.c
    $(CC) $(CFLAGS) set.c -o set

clean:
    rm -f init destroy check set