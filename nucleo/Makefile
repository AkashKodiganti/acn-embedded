# This file was automagically generated by mbed.org. For more information, 
# see http://mbed.org/handbook/Exporting-to-GCC-ARM-Embedded

default: all

APP_NAME = mbed_arrow
APP_OUT = ${APP_NAME}.out

GCC_BIN = 
PROJECT = arrow

C_SDK_PATH = $(PWD)/../acn-sdk-c
C_SDK_MAKE_PATH = $(C_SDK_PATH)/platforms/nucleo

-include $(C_SDK_MAKE_PATH)/Makefile.mbed.def

CPPSRCS  = $(wildcard $(PWD)/*.cpp)
CPPSRCS += $(wildcard $(PWD)/DnsQuery/*.cpp)
CPPSRCS += $(shell find $(PWD)/X_NUCLEO_IDW01M1 -name "*.cpp")
CPPSRCS += $(shell find $(PWD)/Components -name "*.cpp")
CPPSRCS += $(shell find $(PWD)/NetworkSocketAPI -name "*.cpp")
CSRCS += $(shell find $(PWD)/X_NUCLEO_IDW01M1 -name "*.c")
CSRCS += $(wildcard $(PWD)/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/arrow/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/bsd/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/time/*.c)
CSRCS += $(wildcard $(PWD)/acnsdkc/json/*.c)

COBJS := $(CSRCS:%.c=%.o)
CPPOBJS := $(CPPSRCS:%.cpp=%.o)
CPPOBJS += $(COBJS)
CPPDEPS := $(CPPSRCS:%.cpp=%.d)

SYS_OBJECTS = $(wildcard $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/*.o)

INCLUDES += -I$(PWD) \
            -I$(PWD)/mbed \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4 \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TARGET_STM/TARGET_STM32F4/TARGET_NUCLEO_F401RE \
            -I$(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM

INCLUDES += -I$(C_SDK_MAKE_PATH)

LIBRARY_PATHS = -L$(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM -L$(PWD)/.build
LIBRARIES = -lmbed -lacnsdkc
LINKER_SCRIPT = $(PWD)/mbed/TARGET_NUCLEO_F401RE/TOOLCHAIN_GCC_ARM/STM32F401XE.ld

# WIFI
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1/Spwf/utils
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1/Spwf_API \
            -I$(PWD)/X_NUCLEO_IDW01M1/Spwf/inc
INCLUDES += -I$(PWD)/X_NUCLEO_IDW01M1/Spwf_API/utils

# NetworkSocketAPI
INCLUDES += -I$(PWD)/NetworkSocketAPI
 
# # IKS
INCLUDES += -I$(PWD)/Components \
            -I$(PWD)/Components/Common \
            -I$(PWD)/Components/Interfaces \
            -I$(PWD)/Components/hts221 \
            -I$(PWD)/X_NUCLEO_COMMON/DevI2C/ \
            -I$(PWD)/X_NUCLEO_COMMON/DevSPI \
            -I$(PWD)/X_NUCLEO_COMMON/DbgMCU \
            -I$(PWD)/TARGET_NUCLEO_F401RE

# DNS
INCLUDES += -I$(PWD)/DnsQuery

INCLUDES += -I$(C_SDK_PATH) \
	    -I$(C_SDK_PATH)/include


ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),mkdbg)
ifdef DEPS
sinclude $(DEPS)
endif
endif
endif

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) $< -o $@
.cpp.o:
	$(CPP) $(CPPC_FLAGS) $(CC_SYMBOLS) $(INCLUDES) $< -o $@

libacnsdkc.a:
	@echo $(PWD)
	if [ ! -e $(PWD)/.build ]; then mkdir $(PWD)/.build; fi
	cd $(C_SDK_PATH) && $(MAKE) -f $(C_SDK_MAKE_PATH)/Makefile.mbed LIBDIR="$(PWD)/.build" CPATH="$(PWD)"

$(APP_OUT): $(CPPOBJS) $(SYS_OBJECTS) $(LINKER_SCRIPT) libacnsdkc.a
	  @echo "build $(APP_OUT)"
	  $(LD) $(LD_FLAGS) -T$(filter %.ld, $^) $(LIBRARY_PATHS) -o $@ $(filter %.o, $^) -Wl,--start-group $(LIBRARIES) $(LD_SYS_LIBS) -Wl,--end-group
	
clean:
	$(RM) $(OBJDIR) 
	$(RM) $(CPPOBJS)
	cd $(C_SDK_PATH) && $(MAKE) -f $(C_SDK_MAKE_PATH)/Makefile.mbed clean LIBDIR="$(PWD)/.build" CPATH="$(PWD)"
	
$(APP_OUT).bin: $(APP_OUT)
	$(OBJCOPY) -O binary $< $@

$(APP_OUT).hex: $(APP_OUT)
	@$(OBJCOPY) -O ihex $< $@

$(APP_OUT).lst: $(APP_OUT)
	@$(OBJDUMP) -Sdh $< > $@

lst: $(APP_OUT).lst

size: $(APP_OUT)
	$(SIZE) $(APP_OUT)
	
flash:
	st-flash write $(APP_OUT).bin 0x8000000

all: libacnsdkc.a $(CPPOBJS) $(APP_OUT) $(APP_OUT).bin
