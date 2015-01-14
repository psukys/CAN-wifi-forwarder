#---
# Makefile for OBD2-wifi-forwarder project
#---
TARGET = obd2_wifi_forwarder

CC = g++
CFLAGS  = -Wall -I. -ggdb
LINKER  = g++ -o
LFLAGS  = -Wall -I. -lm
SRCDIR  = src
OBJDIR  = obj
BINDIR  = bin
TESTDIR = tests

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

TESTSRC  := $(wildcard $(TESTDIR)/*.cpp)
TESTOBJ  := $(TESTSRC:$(TESTDIR)/%.cpp=$(TESTDIR)/%.o)
#remove an object that already has a main method
TESTDEPS := $(filter-out obj/main.o, $(OBJECTS))
TESTS    := $(TESTSRC:$(TESTDIR)/%.cpp=$(TESTDIR)/%.test)
rm        = rm -f
create_dir=@mkdir -p $(@D)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(create_dir)
	@$(LINKER) $@ $(LFLAGS) $(OBJECTS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(create_dir)
	@$(CC) $(CFLAGS) -c $< -o $@

$(TESTS): $(TESTOBJ) $(OBJECTS)
	$(create_dir)
#the patsubst part is pretty hacky
	@$(LINKER) $@ $(LFLAGS) $(patsubst %.test, %.o, $@) $(TESTDEPS)

$(TESTOBJ): $(TESTDIR)/%.o : $(TESTDIR)/%.cpp
	$(create_dir)
	@$(CC) $(CFLAGS) -c $< -o $@

tests: $(TESTS) $(TESTOBJ)
	@for test in $(TESTS); \
	do \
		./$$test; \
	done

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@$(rm) $(TESTS)
	@$(rm) $(TESTOBJ)

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
