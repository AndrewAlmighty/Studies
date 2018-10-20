CC = cc
CFLAGS = -Wall -pedantic-errors -Werror
LD = gcc

BUILDDIR = build
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, main.o miscellaneous_methods.o networkmethods.o tanenbaum.o)
HEADERS = message.h miscellaneous_methods.h networkmethods.h tanenbaum.h

vpath %.c src
vpath %.h src

all: $(BUILDDIR)/Tanenbaum_Algorithm

$(BUILDDIR)/Tanenbaum_Algorithm: $(OBJS) | $(BUILDDIR)
	$(LD) $^ -o $@

$(OBJS) : $(OBJDIR)/%.o : %.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY:
	clean

clean:
	rm -f build/Tanenbaum_Algorithm
	rm -f obj/*
