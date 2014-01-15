LD=gcc

LIBS=
FLAGS=-std=c99 -fgnu89-inline -Wall -g

LDFLAGS=$(FLAGS) $(LIBS)

TARGET=client server 
SRCS=$(addprefix src/, $(addsuffix .c, $(TARGET)))

all: $(TARGET)

%: src/%.c
	$(LD) $< $(LDFLAGS) -o $@

clean:
	rm -rf $(TARGET)