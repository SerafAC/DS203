CXX=i686-w64-mingw32-g++
TOP=../..
SRCDIR=$(TOP)/Source
SRC:=$(SRCDIR)/User/_Modules.cpp $(shell find $(SRCDIR)/Core $(SRCDIR)/Framework/ $(SRCDIR)/Gui/ $(SRCDIR)/HwLayer/Win32/ $(SRCDIR)/Library/ $(SRCDIR)/Main/  -name *.cpp) 
SRCDIRS=$(sort $(dir $(OBJ)))
OBJ:=$(addprefix obj/,$(patsubst $(SRCDIR)/%.cpp,%.o,$(SRC)))
OBJDIRS=$(sort $(dir $(OBJ)))
SRC_INCLUDES=$(addprefix -I,$(SRCDIRS))

obj/%.o : $(SRCDIR)/%.cpp
	$(CXX) -g -c -D_WINDOWS -Iinc -I$(TOP) $^ -o $@

all: outdirs dso203.exe

outdirs: 
	mkdir -p $(OBJDIRS)

dso203.exe: $(OBJ)
	$(CXX) -static -g $(OBJ) -o dso203.exe -lgdi32
	
clean:
	rm -r dso203.exe obj
