CC := g++
CFLAGS := -O3
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))

$(TARGETDIR)/checkers: $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	g++ -O3 $(BUILDDIR)/ai.o $(BUILDDIR)/board.o $(BUILDDIR)/checkers.o $(BUILDDIR)/hash.o -o $(TARGETDIR)/checkers

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<"; $(CC) $(CFLAGS) -c -o $@ $<

debug:
	g++ -g $(SRCDIR)/checkers.cpp $(SRCDIR)/ai.cpp $(SRCDIR)/board.cpp $(SRCDIR)/hash.cpp -o $(BUILDDIR)/checkersDebug

clean:
	rm $(SRCDIR)/*.o $(SRCDIR)/*.exe
