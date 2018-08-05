CC = gcc
CXX = g++
LD = gcc

CFLAGS = -O0
CXXFLAGS = -O0
LDFLAGS =

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

test: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm test $(OBJS) $(DEPS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(DEPS)
