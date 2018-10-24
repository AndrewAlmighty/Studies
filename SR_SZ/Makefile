CC = cc
LD = gcc
GCC_ARM = arm-linux-gnueabi-gcc

CFLAGS = -Wall -pedantic-errors -Werror

BUILDDIR = build
OBJDIR_X86 = obj_x86
OBJDIR_ARM = obj_arm
OBJS_X86 = $(addprefix $(OBJDIR_X86)/, $(OBJS))
OBJS_ARM = $(addprefix $(OBJDIR_ARM)/, $(OBJS))
OBJS = main.o miscellaneous_methods.o networkmethods.o tanenbaum.o
HEADERS = message.h miscellaneous_methods.h networkmethods.h tanenbaum.h

vpath %.c src
vpath %.h src

all: x86_64 arm

x86_64: $(BUILDDIR)/Tanenbaum_Algorithm_x86_64

arm: $(BUILDDIR)/Tanenbaum_Algorithm_Arm

$(BUILDDIR)/Tanenbaum_Algorithm_x86_64: $(OBJS_X86) | $(BUILDDIR)
	$(LD) $^ -o $@

$(BUILDDIR)/Tanenbaum_Algorithm_Arm: $(OBJS_ARM) | $(BUILDDIR)
	$(GCC_ARM) $^ -o $@ -static

$(OBJS_X86): $(OBJDIR_X86)/%.o : %.c $(HEADERS) | $(OBJDIR_X86)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_ARM): $(OBJDIR_ARM)/%.o : %.c $(HEADERS) | $(OBJDIR_ARM)
	$(GCC_ARM) -c $< -o $@

$(OBJDIR_X86):
	mkdir $(OBJDIR_X86)

$(OBJDIR_ARM):
	mkdir $(OBJDIR_ARM)

$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY:
	clean

clean:
	rm -rf build
	rm -rf obj_x86
	rm -rf obj_arm
