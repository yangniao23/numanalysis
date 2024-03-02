NAME = k5
TARGET = $(NAME).exe
SRCS = src\$(NAME).c src\lu_decomposition.c
OBJS = $(SRCS:.c=.o)
CC = gcc
CFLAGS = -O2 -march=native
INCLUDES = -Ilib -Isrc
LDFLAGS = -Llib
RM = del /F /Q

all: $(TARGET)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(INCLUDES)

clean:
	$(RM) $(TARGET) $(OBJS)
