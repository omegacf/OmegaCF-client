CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TARGET := bin/omegaAlpha
HEURISTICTARGET := bin/heuristicTest
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -std=c++17 -O3 -Wall
LIB := -L lib -ltensorflow
INC := -I include

INC_HT := $(INC)
TARGET_HT := bin/heuristicTest
SOURCES_HT := $(wildcard test/heuristic/*.cpp)
OBJECTS_HT := $(filter-out build/main.o, $(OBJECTS)) $(SOURCES_HT:test/heuristic/%.cpp=build/heuristic/%.o)


$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ $(LIB) -o $(TARGET) 
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR) $(BUILDDIR)/game $(BUILDDIR)/network $(BUILDDIR)/player $(BUILDDIR)/util $(BUILDDIR)/factory $(BUILDDIR)/ml
	@$(CC) $(CFLAGS) $(LIB) $(INC) -c -o $@ $< && echo "[OK] $@"

$(TARGET_HT): $(OBJECTS_HT)
	@mkdir -p $(BINDIR)
	@echo "[OK] $@" && $(CC) $(CFLAGS) $^ -o $@


build/heuristic/main.o: test/heuristic/main.cpp
	@mkdir -p $(BUILDDIR) $(BUILDDIR)/heuristic
	@$(CC) $(CFLAGS) -c $< $(INC_HT) -o $@ && echo "[OK] $@"

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR)"; $(RM) -r $(BUILDDIR) $(BINDIR)

# Tests
test:
	$(CC) test/heuristic/main.cpp -o bin/tester $(LIB)

	# Tests
heuristic: $(TARGET_HT)

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean

all: $(TARGET)
