# ARGUMENTS
TARGET_NAME=gravitee

ifneq ($(TEST),)
ifeq ($(findstring test_,$(TEST)),)
$(error "TEST=$(TEST) is invalid (test function must start with test_)")
else
TARGET_NAME=$(TEST)
endif
endif

# DIRECTORIES
MSPGCC_ROOT_DIR = /home/ibrahim/ti/msp430-gcc
MSPGCC_BIN_DIR = $(MSPGCC_ROOT_DIR)/bin
MSPGCC_INCLUDE_DIR = $(MSPGCC_ROOT_DIR)/include

INCLUDE_DIRS = $(MSPGCC_INCLUDE_DIR) \
			   ./src \
			   ./src/drivers \
			   ./src/common  \

LIB_DIRS = $(MSPGCC_INCLUDE_DIR)
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
# BIN_DIR = $(BUILD_DIR)/bin
TI_CCS_DIR = /home/ibrahim/ti/ccs2020/ccs
DEBUG_BIN_DIR = $(TI_CCS_DIR)/ccs_base/DebugServer/bin
DEBUG_DRIVERS_DIR = $(TI_CCS_DIR)/ccs_base/DebugServer/drivers

# TOOLCHAIN
CC = $(MSPGCC_BIN_DIR)/msp430-elf-gcc
RM = rm
DEBUG = DYLD_LIBRARY_PATH=$(DEBUG_DRIVERS_DIR) $(DEBUG_BIN_DIR)/mspdebug
CPPCHECK = cppcheck
SIZE = $(MSPGCC_BIN_DIR)/msp430-elf-size
READELF = $(MSPGCC_BIN_DIR)/msp430-elf-readelf

# FILES
TARGET = $(BUILD_DIR)/bin/$(TARGET_NAME)

ifndef TEST
MAIN_FILE = src/main.c
else
MAIN_FILE = src/test/test.c
endif

SOURCES_WITH_HEADERS = \
		src/drivers/led.c \
		src/drivers/gpio.c

SOURCES = \
		$(MAIN_FILE) \
		$(SOURCES_WITH_HEADERS)

OBJECT_NAMES = $(SOURCES:.c=.o)
OBJECTS = $(patsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))


# DEFINES
DEFINES = $(addprefix -DTEST=,$(TEST))


# FLAGS
MCU = msp430g2553
WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mmcu=$(MCU) $(WFLAGS) $(DEFINES) $(addprefix -I, $(INCLUDE_DIRS)) -Og -g
LDFLAGS = -mmcu=$(MCU) $(DEFINES) $(addprefix -L, $(LIB_DIRS))


# BUILD
## LINKING
$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@

## COMPILING
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# PHONIES
.PHONY: all clean flash cppcheck size symbols

all: $(TARGET)

clean:
	$(RM) -r $(BUILD_DIR)

flash: $(TARGET)
	sudo $(DEBUG) --allow-fw-update tilib "prog $(TARGET)"

cppcheck:
	@$(CPPCHECK) --quiet --enable=all --error-exitcode=1 \
	--inline-suppr \
	-I $(INCLUDE_DIRS) \ 
	$(SOURCES)

size: $(TARGET)
	@$(SIZE) $(TARGET)

symbols: $(TARGET)
	# List symbols table sorted by size
	@$(READELF) -s $(TARGET) | sort -n -k3

# Use -i tag to exclude directory from cppcheck
# -d /dev/ttyACM0
