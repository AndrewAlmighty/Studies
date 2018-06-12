CC = g++
FLAGS = -Wall -pedantic-errors -ggdb -std=c++11

BUILDDIR = build

PROCESSOR_SRC = src/processor
PROCESS_SRC = src/process
PROCESSLIST_SRC = src/processList
PORTICO_INC= ~/workspace/portico/include/hla13

all: $(BUILDDIR)/processor $(BUILDDIR)/process $(BUILDDIR)/processList $(BUILDDIR)/fom.fed

$(BUILDDIR)/processor: $(PROCESSOR_SRC)/*.cpp $(PROCESSOR_SRC)/*.h | $(BUILDDIR)
	$(CC) $(FLAGS) -I $(PORTICO_INC) $^ -o $@

$(BUILDDIR)/process: $(PROCESS_SRC)/*.cpp $(PROCESS_SRC)/*.h | $(BUILDDIR)
	$(CC) $(FLAGS) -I $(PORTICO_INC) $^ -o $@

$(BUILDDIR)/processList: $(PROCESSLIST_SRC)/*.cpp $(PROCESSLIST_SRC)/*.h | $(BUILDDIR)
	$(CC) $(FLAGS) -I $(PORTICO_INC) $^ -o $@

$(BUILDDIR)/fom.fed: src/fom.fed | $(BUILDDIR)
	cp src/fom.fed $(BUILDDIR)/fom.fed

#if there are no build directory, make it

$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY:
	clean

clean:
	rm -f build/* 
