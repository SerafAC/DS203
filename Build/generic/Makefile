TOP=../..
SRCDIR=$(TOP)/Source

PLATFORM=dso

ifeq ($(PLATFORM),Win32)
	TC=i686-w64-mingw32-
	TARGET=dso203.exe
	HARDWARE=Win32
	CXXFLAGS=-Og -g -D_WINDOWS -Iinc -I$(TOP)
else ifeq ($(PLATFORM),dso)
	TC=arm-none-eabi-
	SLOT=3
	TARGET=dso
	HARDWARE=ArmM3
	INCLUDES=-I$(TOP) -I$(SRCDIR)/HwLayer/$(HARDWARE)/stm32f10x/inc -I$(SRCDIR)/HwLayer/$(HARDWARE)/src
	ARCH:=-mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd 
	AFLAGS:=$(ARCH)	
	CFLAGS=-Wall -Os -fno-common $(ARCH) -msoft-float -MD -D _ARM -Wno-psabi $(INCLUDES) 	
	CXXFLAGS=$(CFLAGS) -fvtable-gc -fno-exceptions -fno-rtti -fno-threadsafe-statics
	
	LDFLAGS=-Wl,-Map=$(TARGET).map  -nostartfiles $(ARCH) -msoft-float -lc -lgcc -L$(SRCDIR)/HwLayer/ArmM3/lds -T app_$(SLOT).lds 
	EXTRA_SRC=$(SRCDIR)/HwLayer/ArmM3/stm32f10x/src/stm32f10x_nvic.c $(SRCDIR)/HwLayer/ArmM3/src/interrupt.c $(SRCDIR)/HwLayer/ArmM3/src/startup.c $(SRCDIR)/HwLayer/ArmM3/stm32f10x/asm/cortexm3_macro.s
else 
$(error unknown platform '$(PLATFORM)')
endif

CXX=$(TC)g++
CC=$(TC)gcc
OBJCOPY=$(TC)objcopy

# -- Source files --
SRC=$(SRCDIR)/User/_Modules.cpp $(shell find $(SRCDIR)/Core $(SRCDIR)/Framework/ $(SRCDIR)/Gui/ $(SRCDIR)/HwLayer/$(HARDWARE)/ $(SRCDIR)/Library/ $(SRCDIR)/Main/  -name *.cpp) $(EXTRA_SRC)
OBJ=$(addprefix build/$(PLATFORM)/obj/, $(addsuffix .o,$(basename $(patsubst $(SRCDIR)/%,%,$(SRC)))))

SRCDIRS=$(sort $(dir $(OBJ)))
OBJDIRS=$(sort $(dir $(OBJ)))
SRC_INCLUDES=$(addprefix -I,$(SRCDIRS))
OBJDIR=build/$(PLATFORM)/obj
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
	
%.elf: outdirs $(OBJ) BIOS.o
	$(CC) -o $@ $(OBJ) $(LDFLAGS) 

all: info outdirs $(TARGET)
	
info:
	@echo "building for $(PLATFORM)"
	@echo "------------"
	@echo "SRC=$(SRC)"
	@echo "------------"
	@echo "OBJ=$(OBJ)"
	@echo "------------"
	

outdirs: 
	mkdir -p $(OBJDIRS)

dso203.exe: $(OBJ)
	$(CXX) -static -g $(OBJ) -o dso203.exe -lgdi32

dso: dso_$(SLOT).hex dso_$(SLOT).elf

# BIOS.o is named in main.lds and should be in the build directory.
BIOS.o: $(SRCDIR)/HwLayer/ArmM3/src/BIOS.S
	$(CC) -c $(AFLAGS) $^ -o $@
		
	
clean:
	rm -r build dso* *.o
