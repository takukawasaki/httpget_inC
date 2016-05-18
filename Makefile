PROGRAM = httpget
OBJS = function.o httpget.o
SRCS = $(OBJS:%.o=%.c)
CFLAGS = -g -Wall -O2
LDFLAGS = 
CC = gcc


$(PROGRAM):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJS) $(LDLIBS)
