CC := g++
AR := ar
# CC := clang --analyze

MAIN := main
LIB := lib
TEST := test

SRCEXT := cpp

# Directories
BINDIR := bin
BUILDDIR := build
INCLUDEDIR := include
LIBDIR := lib
SRCDIR := .
TESTDIR := test

CFLAGS := -Wno-narrowing # -Wall -g
LIBRARY := `sdl2-config --libs` -lSDL2_image
INCLUDE := -I $(INCLUDEDIR) `sdl2-config --cflags`

# Ignore $(MAIN).$(SRCEXT), test.c, and any $(SRCEXT) files starting with an underscore.
SOURCES := $(shell find $(SRCDIR) -type f ! -name ".*" ! -name $(MAIN).$(SRCEXT) ! -name $(TEST).$(SRCEXT) ! -name _* -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

.PHONY: clean test


all: build

build: $(BINDIR)/$(MAIN)

clean:
	@echo '[+] Cleaning'
	$(RM) -rf -- $(BINDIR) $(BUILDDIR)

expand: $(SRCDIR)/$(MAIN).$(SRCEXT)
	@echo '[+] Expanding'
	$(CC) $(INCLUDE) -E $^

run: build
	@echo '[+] Running'
	@exec ./$(BINDIR)/$(MAIN)

test: $(BINDIR)/$(TEST)


$(BINDIR)/$(MAIN): $(SRCDIR)/$(MAIN).$(SRCEXT) $(OBJECTS)
	@echo '[+] Building'
	@mkdir -pv $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^ $(LIBRARY)

$(BINDIR)/$(TEST): $(TESTDIR)/$(TEST).$(SRCEXT) $(OBJECTS)
	@echo '[+] Testing'
	@mkdir -pv $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^ $(LIBRARY)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo '[+] Compiling'
	@mkdir -pv $(shell dirname $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<
