INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -include stdio.h
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = task3_list.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

SOURCES = task3_menu.c task3_list.c
_OBJS = $(SOURCES:.c=.o)
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task3: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)
.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o ~ core $(INCLDIR)/~
