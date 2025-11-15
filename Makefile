CC=gcc
CFLAGS=-Wall -Wextra -Iinclude
LDFLAGS=-pthread -lncurses

SRC = src/main.c \
      src/speed.c \
      src/fuel.c \
      src/temp.c \
      src/logger.c \
      src/utils.c \
      src/ui.c

all: vehicle_monitor

vehicle_monitor: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o vehicle_monitor $(LDFLAGS)

clean:
	rm -f vehicle_monitor
