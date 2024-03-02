NAME = k9
TARGET = $(NAME).exe
SRCS = src\$(NAME).c lib\NAbasic.c
OBJS = $(SRCS:.c=.o)
CC = gcc
CFLAGS = -Wall -Werror -W -O2 -march=native
INCLUDES = -Ilib
LDFLAGS = -Llib -lm
RM = del /F /Q

all: $(TARGET)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(INCLUDES)

clean:
	$(RM) $(TARGET) $(OBJS)