# Makefile for FOC Hall Correction Machine
# Uses gcc-arm-none-eabi toolchain

# Target name
TARGET = FOC_hall_correction_machine

# Toolchain path (use forward slashes to avoid path issues)
TOOLCHAIN_PATH = E:/tool/compile/gcc-arm-none-eabi-7-2018-q2-update-win32/bin

# Toolchain
CC = "$(TOOLCHAIN_PATH)/arm-none-eabi-gcc.exe"
AS = "$(TOOLCHAIN_PATH)/arm-none-eabi-as.exe"
LD = "$(TOOLCHAIN_PATH)/arm-none-eabi-ld.exe"
OBJCOPY = "$(TOOLCHAIN_PATH)/arm-none-eabi-objcopy.exe"
OBJDUMP = "$(TOOLCHAIN_PATH)/arm-none-eabi-objdump.exe"
SIZE = "$(TOOLCHAIN_PATH)/arm-none-eabi-size.exe"

# Directories
CMSIS_DIR = Mcu/HK32M070/Driver/CMSIS
STD_PERIPH_DIR = Mcu/HK32M070/Driver/HK32M07x_StdPeriphDriver
USER_DIR = User
HWCONTF_DIR = hwconf
MOTOR_DIR = motor
UTIL_DIR = util
STARTUP_DIR = Mcu/HK32M070/Driver/CMSIS/HK32M07x/Source/ARM

# Source files
C_SOURCES = \
$(CMSIS_DIR)/HK32M07x/Source/system_hk32m07x.c \
$(STD_PERIPH_DIR)/src/hk32m07x_gpio.c \
$(STD_PERIPH_DIR)/src/hk32m07x_rcc.c \
$(STD_PERIPH_DIR)/src/hk32m07x_misc.c \
$(USER_DIR)/conf_general.c \
$(HWCONTF_DIR)/hw.c \
$(HWCONTF_DIR)/hw_conf.c \
$(MOTOR_DIR)/foc_hall.c \
$(MOTOR_DIR)/foc_math.c \
$(MOTOR_DIR)/mc_interface.c \
$(MOTOR_DIR)/mcpwm.c \
$(MOTOR_DIR)/mcpwm_foc.c \
$(UTIL_DIR)/utils_math.c \
$(USER_DIR)/main.c \
$(UTIL_DIR)/utils_mem.c \
$(UTIL_DIR)/syscalls.c

# Assembly files
ASM_SOURCES = \
Mcu/HK32M070/Startup/startup_hk32m07x_gnu.s

# Include directories
INCLUDE_DIRS = \
-I$(CMSIS_DIR)/CM0/Core \
-I$(CMSIS_DIR)/HK32M07x/Include \
-I$(STD_PERIPH_DIR)/inc \
-I$(USER_DIR) \
-I$(HWCONTF_DIR) \
-I$(MOTOR_DIR) \
-I$(UTIL_DIR)

# # Defines
DEFINES = \
-DUSE_HAL_DRIVER \
-DHK32M07x \
-DSYSTEM_CORE_CLOCK=64000000

# Compiler flags
CFLAGS = -mcpu=cortex-m0 -mthumb -O0 -Wall -fdata-sections -ffunction-sections
CFLAGS += $(INCLUDE_DIRS) $(DEFINES)

# Assembly flags
ASFLAGS = -mcpu=cortex-m0 -mthumb

# Linker flags
LDFLAGS = -mcpu=cortex-m0 -mthumb -T Mcu/HK32M070/Startup/startup_hk32m07x.sct -Wl,-Map=$(TARGET).map,--cref -Wl,--gc-sections

# Build directory
BUILD_DIR = build

# Object files
OBJECTS = \
$(BUILD_DIR)/startup_hk32m07x.o \
$(BUILD_DIR)/system_hk32m07x.o \
$(BUILD_DIR)/hk32m07x_gpio.o \
$(BUILD_DIR)/hk32m07x_rcc.o \
$(BUILD_DIR)/hk32m07x_misc.o \
$(BUILD_DIR)/conf_general.o \
$(BUILD_DIR)/hw.o \
$(BUILD_DIR)/hw_conf.o \
$(BUILD_DIR)/foc_hall.o \
$(BUILD_DIR)/foc_math.o \
$(BUILD_DIR)/mc_interface.o \
$(BUILD_DIR)/mcpwm.o \
$(BUILD_DIR)/mcpwm_foc.o \
$(BUILD_DIR)/utils_math.o \
$(BUILD_DIR)/main.o \
$(BUILD_DIR)/utils_mem.o \
$(BUILD_DIR)/syscalls.o

# Dependency files
DEPS = $(OBJECTS:.o=.d)

# Default target
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Create build directory
$(BUILD_DIR):
	@mkdir -p "$@"

# Compile C files
$(BUILD_DIR)/system_hk32m07x.o: $(CMSIS_DIR)/HK32M07x/Source/system_hk32m07x.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_adc.o: $(STD_PERIPH_DIR)/src/hk32m07x_adc.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_atu.o: $(STD_PERIPH_DIR)/src/hk32m07x_atu.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_cmp.o: $(STD_PERIPH_DIR)/src/hk32m07x_cmp.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_dbgmcu.o: $(STD_PERIPH_DIR)/src/hk32m07x_dbgmcu.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_divsqrt.o: $(STD_PERIPH_DIR)/src/hk32m07x_divsqrt.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_exti.o: $(STD_PERIPH_DIR)/src/hk32m07x_exti.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_flash.o: $(STD_PERIPH_DIR)/src/hk32m07x_flash.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_gpio.o: $(STD_PERIPH_DIR)/src/hk32m07x_gpio.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_htu.o: $(STD_PERIPH_DIR)/src/hk32m07x_htu.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_i2c.o: $(STD_PERIPH_DIR)/src/hk32m07x_i2c.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_iwdg.o: $(STD_PERIPH_DIR)/src/hk32m07x_iwdg.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_misc.o: $(STD_PERIPH_DIR)/src/hk32m07x_misc.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_pga.o: $(STD_PERIPH_DIR)/src/hk32m07x_pga.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_pwr.o: $(STD_PERIPH_DIR)/src/hk32m07x_pwr.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_rcc.o: $(STD_PERIPH_DIR)/src/hk32m07x_rcc.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_spi.o: $(STD_PERIPH_DIR)/src/hk32m07x_spi.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_syscfg.o: $(STD_PERIPH_DIR)/src/hk32m07x_syscfg.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_uart.o: $(STD_PERIPH_DIR)/src/hk32m07x_uart.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_utu.o: $(STD_PERIPH_DIR)/src/hk32m07x_utu.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_vdiv.o: $(STD_PERIPH_DIR)/src/hk32m07x_vdiv.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hk32m07x_wwdg.o: $(STD_PERIPH_DIR)/src/hk32m07x_wwdg.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/conf_general.o: $(USER_DIR)/conf_general.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/main.o: $(USER_DIR)/main.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hw.o: $(HWCONTF_DIR)/hw.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/hw_conf.o: $(HWCONTF_DIR)/hw_conf.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/foc_hall.o: $(MOTOR_DIR)/foc_hall.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/foc_math.o: $(MOTOR_DIR)/foc_math.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/mc_interface.o: $(MOTOR_DIR)/mc_interface.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/mcpwm.o: $(MOTOR_DIR)/mcpwm.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/mcpwm_foc.o: $(MOTOR_DIR)/mcpwm_foc.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@



$(BUILD_DIR)/utils_math.o: $(UTIL_DIR)/utils_math.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/utils_mem.o: $(UTIL_DIR)/utils_mem.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/syscalls.o: $(UTIL_DIR)/syscalls.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Compile assembly files
$(BUILD_DIR)/startup_hk32m07x.o: Mcu/HK32M070/Startup/startup_hk32m07x_gnu.s | $(BUILD_DIR)
	@echo "Assembling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@echo "Linking..."
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

# Create hex file
$(BUILD_DIR)/$(TARGET).hex:
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(TARGET).elf $@

# Create bin file
$(BUILD_DIR)/$(TARGET).bin:
	$(OBJCOPY) -O binary $(BUILD_DIR)/$(TARGET).elf $@

# Clean
clean:
	@echo "Cleaning..."
	@if [ -d "$(BUILD_DIR)" ]; then rm -rf "$(BUILD_DIR)"; fi
	@if [ -f "$(TARGET).map" ]; then rm "$(TARGET).map"; fi

# Include dependencies
-include $(DEPS)

# Phony targets
.PHONY: all clean