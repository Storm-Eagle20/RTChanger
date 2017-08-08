#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
TARGET		:= $(notdir $(TOPDIR))
BUILD		:= build
SOURCES		:= source
DATA		:= data
INCLUDES	:= inc
ROMFS		:= 
APP_TITLE       := RTChanger
APP_DESCRIPTION := A tool which can change the Raw RTC to bypass timegates!
APP_AUTHOR      := /u/X-The-Komujin
APP_PRODUCT_CODE:= CTR-P-ARCA
APP_UNIQUE_ID   := 0xFCCC
ICON            := assets/logo.png

APP_TITLE	:= $(shell echo "$(APP_TITLE)" | cut -c1-128)
APP_DESCRIPTION	:= $(shell echo "$(APP_DESCRIPTION)" | cut -c1-256)
APP_AUTHOR	:= $(shell echo "$(APP_AUTHOR)" | cut -c1-128)
APP_PRODUCT_CODE:= $(shell echo $(APP_PRODUCT_CODE) | cut -c1-16)
APP_UNIQUE_ID	:= $(shell echo $(APP_UNIQUE_ID) | cut -c1-7)

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard

CFLAGS	:=	-g -Wall -O2 -mword-relocations \
			-fomit-frame-pointer -ffast-math \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM11 -D_3DS

CXXFLAGS:=	$(CFLAGS) -Wno-reorder -fno-rtti -std=gnu++11

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS	:=	-lctru -lm

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(CTRULIB)


#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/out/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES) $(BUILD)/_lzz_temp,$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(shell find $(SOURCES) -name '*.c' -printf "%P\n")
CPPFILES	:=	$(shell find $(SOURCES) -name '*.cpp' -printf "%P\n")
LPPFILES	:=	$(shell find $(SOURCES) -name '*.lpp' -printf "%P\n")
SFILES		:=	$(shell find $(SOURCES) -name '*.s' -printf "%P\n")
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

LPPSOOS		:=	$(foreach fil,$(LPPFILES),$(patsubst %.lpp,%.cpp,$(fil)))
CPPFILES	+=	$(LPPSOOS)

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)/include) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			$(foreach dir,$(SOURCES) $(BUILD)/_lzz_temp,-I$(CURDIR)/$(dir)) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
			$(foreach dir,$(INCLUDES),-L$(CURDIR)/$(dir)/lib)

ifeq ($(strip $(ICON)),)
	icons := $(wildcard *.png)
	ifneq (,$(findstring $(TARGET).png,$(icons)))
		export APP_ICON := $(TOPDIR)/$(TARGET).png
	else
		ifneq (,$(findstring icon.png,$(icons)))
			export APP_ICON := $(TOPDIR)/icon.png
		endif
	endif
else
	export APP_ICON := $(TOPDIR)/$(ICON)
endif


export _3DSXFLAGS += --smdh=$(TOPDIR)/$(BUILD)/_smdh.bin

ifneq ($(strip $(ROMFS)),)
	export _3DSXFLAGS += --romfs=$(ROMFS)
endif

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
all: $(BUILD)

$(LPPSOOS): $(LPPFILES)
	@mkdir -p $(BUILD)/_lzz_temp/$(shell dirname $@)
	@echo [LZZ] $@
	@lzz -hx hpp -hd -sd -c -o $(BUILD)/_lzz_temp/$(shell dirname $@) $(CURDIR)/soos/$(patsubst %.cpp,%.lpp,$@)

$(BUILD): $(LPPSOOS)
	@[ -d $@ ] || mkdir -p $@
	@[ -d out ] || mkdir -p out
	@find $(SOURCES) -type d -printf "%P\0" | xargs -0 -I {} mkdir -p $(BUILD)/{}
	@[ ! -d $(BUILD)/_lzz_temp ] || find $(BUILD)/_lzz_temp -type d -printf "%P\0" | xargs -0 -I {} mkdir -p $(BUILD)/{}
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -rf $(BUILD) $(TARGET).elf out/


#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
.PHONY: all
all: $(OUTPUT).cia $(OUTPUT).3dsx 

$(OUTPUT).3dsx: $(OUTPUT).elf $(CURDIR)/_smdh.bin

$(OUTPUT).elf: $(OFILES)

$(CURDIR)/_banner.bin: $(TOPDIR)/assets/banner.png $(TOPDIR)/assets/banner.wav
	@bannertool makebanner -i $(TOPDIR)/assets/banner.png -a $(TOPDIR)/assets/banner.wav -o $(CURDIR)/_banner.bin

$(CURDIR)/_smdh.bin: $(TOPDIR)/$(ICON)
	@bannertool makesmdh -s "$(APP_TITLE)" -l "$(APP_DESCRIPTION)" -p "$(APP_AUTHOR)" -i $(TOPDIR)/$(ICON) -o $(CURDIR)/_smdh.bin


$(CURDIR)/_strip.elf: $(OUTPUT).elf
	@cp $(OUTPUT).elf $(CURDIR)/_strip.elf
	@$(PREFIX)strip $(CURDIR)/_strip.elf

$(OUTPUT).cia: $(CURDIR)/_strip.elf $(CURDIR)/_banner.bin $(CURDIR)/_smdh.bin
	@makerom -f cia -o $(OUTPUT).cia -rsf $(TOPDIR)/assets/cia.rsf -target t -exefslogo -elf $(CURDIR)/_strip.elf -icon $(CURDIR)/_smdh.bin -banner $(CURDIR)/_banner.bin -DAPP_TITLE="$(APP_TITLE)" -DAPP_PRODUCT_CODE="$(APP_PRODUCT_CODE)" -DAPP_UNIQUE_ID="$(APP_UNIQUE_ID)"
	@echo "built ... $(notdir $@)"

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o: %.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)


#---------------------------------------------------------------------------------------
-include $(DEPENDS)
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
