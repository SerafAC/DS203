CXX=i686-pc-mingw32-g++
TOP=../..
SRCDIR=$(TOP)/Source
SRC:=$(SRCDIR)/User/_Modules.cpp $(shell find $(SRCDIR)/Core $(SRCDIR)/Framework/ $(SRCDIR)/Gui/ $(SRCDIR)/HwLayer/Win32/ $(SRCDIR)/Library/ $(SRCDIR)/Main/  -name *.cpp) 

SRCDIRS=$(sort $(dir $(OBJ)))

OBJ:=$(addprefix obj/,$(patsubst $(SRCDIR)/%.cpp,%.o,$(SRC)))
OBJDIRS=$(sort $(dir $(OBJ)))

SRC_INCLUDES=$(addprefix -I,$(SRCDIRS))

obj/%.o : $(SRCDIR)/%.cpp
	$(CXX) -c -D_WINDOWS -Iinc -I$(TOP) $^ -o $@

all: outdirs $(OBJ)

outdirs: 
	mkdir -p $(OBJDIRS)
	

info:    
	echo $(SRCDIRS)
	echo $(SRC_INCLUDES)
	echo "SRC=$(SRC)"




	
