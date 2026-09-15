INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -include stdio.h
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

SOURCES = task4_threads.c
_OBJS = $(SOURCES:.c=.o)
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

task4: $(OBJS)
	gcc $(CFLAGS) -pthread -o $@ $^
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o ~ core $(INCLDIR)/~
