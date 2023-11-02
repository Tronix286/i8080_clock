CC   = gcc.exe
bin = i8080_tests
src = $(wildcard *.c)
obj = $(src:.c=.o)
CFLAGS = -g -Wall -Wextra -O2 -std=c99 -pedantic
LDFLAGS = -lmingw32 -lSDLmain -lSDL

.PHONY: all clean

all: $(bin)

$(bin): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	-rm $(bin) $(obj)
