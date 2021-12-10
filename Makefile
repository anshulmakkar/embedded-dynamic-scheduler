##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.13.0-B3] date: [Mon Mar 29 11:24:45 BST 2021] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = kispe-dynlink
TARGET_APP = app_image
TARGET_APP_V1 = app_image_v1
#TARGET_DMP = app_image.ld


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
APP_DIR = App

######################################
# source
#Core/Src/main.c \
#Core/Src/logger.c \
#System/task_manager.c 
######################################
# C sources

C_APP_SOURCES = \
App/simple.c

C_APP_V1_SOURCES = \
App/simple_v1.c

C_SOURCES =  \
Core/Src/main.c \
Core/Src/logger.c \
Core/Src/jumptbl.c \
System/task_manager.c \
System/migrator.c \
Core/Src/stm32h7xx_it.c \
Core/Src/stm32h7xx_hal_msp.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c \
Core/Src/freertos.c \
Core/Src/system_stm32h7xx.c \
Core/Src/stm32h7xx_hal_timebase_tim.c \
Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
Middlewares/Third_Party/FreeRTOS/Source/list.c \
Middlewares/Third_Party/FreeRTOS/Source/queue.c \
Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
Middlewares/Third_Party/FreeRTOS/Source/timers.c \
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_1.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c

# ASM sources
ASM_SOURCES =  \
startup_stm32h753xx.s

ASM_APP_SOURCES = \
$(APP_DIR)/startup_app.s

ASM_APP_V1_SOURCES = \
$(APP_DIR)/startup_app_v1.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
AR = $(GCC_PATH)/$(PREFIX)ar
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
HXDMP = hexdump -v -e '"BYTE(0x" 1/1 "%02X" ")\n"'
#HXDMPV1 = hexdump -v -e '1/1 "%02X"'
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32H753xx \
-DDEBUG


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDrivers/STM32H7xx_HAL_Driver/Inc \
-IDrivers/STM32H7xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32H7xx/Include \
-IDrivers/CMSIS/Include \
-IMiddlewares/Third_Party/FreeRTOS/Source/include \
-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 \
-IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-ISystem/include 
# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -nostdlib

CFLAGS_APP = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -nostdlib

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32H753ZITx_FLASH.ld
LDSCRIPT_APP = app.ld
LDSCRIPT_APPV1 = app_v1.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBS_APP = -lc -lm -lnosys
LIBDIR = ./build
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) -L$(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LDFLAGS_APP = $(MCU) -specs=nano.specs -T$(LDSCRIPT_APP) -L$(LIBDIR) $(LIBS_APP) -Wl,-Map=$(BUILD_DIR)/$(TARGET_APP).map,--cref -Wl,--gc-sections -nostartfiles -fPIC -shared
LDFLAGS_APPV1 = $(MCU) -specs=nano.specs -T$(LDSCRIPT_APPV1) -L$(LIBDIR) $(LIBS_APP) -Wl,-Map=$(BUILD_DIR)/$(TARGET_APP_V1).map,--cref -Wl,--gc-sections -nostartfiles -fPIC -shared

app: $(BUILD_DIR)/$(TARGET_APP).elf $(BUILD_DIR)/$(TARGET_APP).hex $(BUILD_DIR)/$(TARGET_APP).bin $(BUILD_DIR)/$(TARGET_APP).ld 

#######################################
# build the the application binary.
#######################################
# list of app objects.
APP_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_APP_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_APP_SOURCES)))

# list of ASM program objects
APP_OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_APP_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_APP_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -nostdlib -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@ 
	
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET_APP).elf:  $(APP_OBJECTS) Makefile
	$(CC) $(APP_OBJECTS) $(LDFLAGS_APP) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	

$(BUILD_DIR)/%.ld: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HXDMP) $< > $@	
	
	
appv1: $(BUILD_DIR)/$(TARGET_APP_V1).elf $(BUILD_DIR)/$(TARGET_APP_V1).ld 
 
#######################################
# build the the application binary v1.
#######################################
# list of app objects.
APP_V1_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_APP_V1_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_APP_V1_SOURCES)))

APP_V1_OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_APP_V1_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_APP_V1_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -nostdlib -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@ 
	
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
	
$(BUILD_DIR)/$(TARGET_APP_V1).elf:  $(APP_V1_OBJECTS) Makefile
	$(CC) $(APP_V1_OBJECTS) $(LDFLAGS_APPV1) -o $@
	$(SZ) $@

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/lib$(TARGET).a $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the the main system binary
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
	
$(BUILD_DIR)/lib$(TARGET).a: $(OBJECTS) Makefile
	$(AR) rcs  $@ $(OBJECTS)
	$(SZ) $@
	
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir $@	
	
#$(BUILD_DIR)/%.ld: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
#	$(HXDMPV1) $< > $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
