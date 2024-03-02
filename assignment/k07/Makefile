NAME = k7
TARGET = $(NAME).exe
SRCS = src\$(NAME).c lib\LU_decomposition.c lib\NAbasic.c
OBJS = $(SRCS:.c=.o)
CC = gcc
CFLAGS = -Wall -Werror -W -O2 -march=native
INCLUDES = -Ilib -Isrc
LDFLAGS = -Llib -lm
RM = del /F /Q

all: $(TARGET)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(INCLUDES)

clean:
	$(RM) $(TARGET) $(OBJS)