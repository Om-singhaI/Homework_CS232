CC = gcc
CFLAGS = -g
OBJS = crawler.o indexPage.o

all: webSearch
webSearch: webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@ -Wall -lm
%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm *.o *~ webSearch
