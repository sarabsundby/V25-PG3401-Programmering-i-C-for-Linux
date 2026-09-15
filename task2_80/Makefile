INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -include stdio.h
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = task2_hash.h task2_count.h task2_sum.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

SOURCES = task2_hash.c task2_count.c task2_sum.c main.c
_OBJS = $(SOURCES:.c=.o)
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task2: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o ~ core $(INCLDIR)/~
