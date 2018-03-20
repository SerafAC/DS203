TOP=..
SRCDIR=$(TOP)/Source

PLATFORM=dso

ifeq ($(PLATFORM),Win32)
	TC        = x86_64-w64-mingw32-
	TARGET    = dso203.exe
	HARDWARE  = Win32
	CXXFLAGS  = --std=gnu++11 -Og -g -D_WINDOWS -Iinc -I$(TOP)
else ifeq ($(PLATFORM),dso)
	TC        = arm-none-eabi-
	SLOT      = 3
	TARGET    = dso
	HARDWARE  = ArmM3
	INCLUDES  = -I$(TOP) -I$(SRCDIR)/HwLayer/$(HARDWARE)/stm32f10x/inc -I$(SRCDIR)/HwLayer/$(HARDWARE)/src
	ARCH      = -mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd 
	AFLAGS    = $(ARCH)	
	CFLAGS    = $(ARCH) -Wall -Os -fno-common -msoft-float -MD -D _ARM -Wno-psabi $(INCLUDES) 	
	CXXFLAGS  = $(CFLAGS) --std=gnu++11 -fno-exceptions -fno-rtti -fno-threadsafe-statics
	
	LDFLAGS   = $(ARCH) -Wl,-Map=$(TARGET).map  -nostartfiles -msoft-float -lc -lgcc -L$(SRCDIR)/HwLayer/ArmM3/lds -T app_$(SLOT).lds 
	EXTRA_SRC = $(SRCDIR)/HwLayer/ArmM3/stm32f10x/src/stm32f10x_nvic.c $(SRCDIR)/HwLayer/ArmM3/src/interrupt.c $(SRCDIR)/HwLayer/ArmM3/src/startup.c $(SRCDIR)/HwLayer/ArmM3/stm32f10x/asm/cortexm3_macro.s
else ifeq ($(PLATFORM),desktop)
	TC        = 
	TARGET    = dso203
	HARDWARE  = SDL2
	PKG_CFLAGS:= $(shell sdl2-config --cflags)
	PKG_LDFLAGS:= $(shell sdl2-config --libs)
	CXXFLAGS  = --std=gnu++11 -Og -g -D_SDL2 -Iinc -I$(TOP) $(PKG_CFLAGS)
	LDFLAGS   = $(PKG_LDFLAGS)
else 
$(error unknown platform '$(PLATFORM)')
endif

CXX           = $(TC)g++
CC            = $(TC)gcc
OBJCOPY       = $(TC)objcopy
STRIP         = $(TC)strip

# -- Source files --
OBJDIR        = build/$(PLATFORM)/obj
SRC           = $(SRCDIR)/User/_Modules.cpp $(shell find $(SRCDIR)/Core $(SRCDIR)/Framework/ $(SRCDIR)/Gui/ $(SRCDIR)/HwLayer/Common $(SRCDIR)/HwLayer/$(HARDWARE)/ $(SRCDIR)/Library/ $(SRCDIR)/Main/  -name *.cpp) $(EXTRA_SRC)
OBJ           = $(addprefix $(OBJDIR)/, $(addsuffix .o,$(basename $(patsubst $(SRCDIR)/%,%,$(SRC)))))
OBJDIRS       = $(sort $(dir $(OBJ)))
# --

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.s
	$(CC) -c $(AFLAGS) $^ -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.S
	$(CC) -c $(AFLAGS) $^ -o $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@
	
%.elf: $(OBJ) BIOS.o
	$(CC) $(LDFLAGS) \
	-o $@ \
	$(OBJ)
	 

.PHONY: all
all: $(TARGET)
	
.PHONY: info
info:
	@echo "building for $(PLATFORM)"
	@echo "------------"
	@echo "SRC=$(SRC)"
	@echo "------------"
	@echo "OBJ=$(OBJ)"
	@echo "------------"
	

$(OBJ): | $(OBJDIRS)
$(OBJDIRS):
	@mkdir -p $(OBJDIRS)
	
	
dso203.exe: $(OBJ)
	$(CXX) -static -g $(OBJ) -o dso203.exe -lgdi32

dso203: $(OBJ)
	$(CXX) -g $(OBJ) -o dso203 $(LDFLAGS)

.PHONY: dso
dso: dso_$(SLOT).hex dso_$(SLOT).elf

# BIOS.o is named in main.lds and m be in the build directory.
BIOS.o: $(SRCDIR)/HwLayer/ArmM3/src/BIOS.S
	$(CC) -c $(AFLAGS) $^ -o $@
		
	
.PHONY: clean
clean:
	rm -r build dso* *.o

.PHONY: win
win:
	$(MAKE) PLATFORM=Win32
desktop:
	$(MAKE) PLATFORM=desktop

cmakefile:
	@echo "cmake_minimum_required (VERSION 3.5)"
	@echo "project(dso)"
	@echo "include_directories($(TOP))"
	@echo "add_executable(dso WIN32 $(SRC))"
