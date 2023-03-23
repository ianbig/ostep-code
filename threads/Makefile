CC = gcc
CFLAGS = -pthread -Wall -Werror -Wextra -g
TARGET = counter-simple counter-scale
.PHONY= all clean


all: $(TARGET)
counter-simple: concurret-counter-simple.c counter-simple.h
	$(CC) $(CFLAGS) -o $@ concurret-counter-simple.c
counter-scale: counter-scale.c counter-scale.h
	$(CC) $(CFLAGS) -o $@ counter-scale.c

clean:
	rm $(TARGET)
