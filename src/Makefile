CC = gcc
OPT = -O0
CFLAGS = -Wall -Wextra -std=c99 -g $(OPT)

TARGET = rush
SRCS = main.c constants.c exec_command.c builtin_commands.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
