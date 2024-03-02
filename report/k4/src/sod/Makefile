NAME = sod
TARGET = $(NAME).out
SRCS = $(NAME).c ../../../../lib/NAbasic.c
OBJS = $(SRCS:.c=.o)
CC = gcc
CFLAGS = -Wall -Werror -g -W -O0 
INCLUDES = -I../../../../lib
LDFLAGS = -L../../../../lib -lm

all: $(TARGET)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(INCLUDES)

clean:
	$(RM) $(TARGET) $(OBJS)
