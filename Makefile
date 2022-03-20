TARGET = make103
BUILD_DIR = build

C_SOURCES2 =  \
Libraries/STM32F4xx_StdPeriph_Driver/src/misc.c \
Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c \
Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c \
Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
FreeRTOS/croutine.c \
FreeRTOS/event_groups.c \
FreeRTOS/list.c \
FreeRTOS/queue.c \
FreeRTOS/tasks.c \
FreeRTOS/timers.c \
FreeRTOS/portable/MemMang/heap_4.c \
FreeRTOS/portable/GCC/ARM_CM3/port.c \
User/bsp/system_stm32f4xx.c \
User/bsp/bsp.c \
User/bsp/stm32f4xx_it.c \
User/bsp/stm32f4xx_assert.c \
User/bsp/src/bsp_key.c \
User/bsp/src/bsp_led.c \
User/bsp/src/bsp_uart_fifo.c \
User/bsp/src/bsp_tim_pwm.c \
User/main.c \
User/SysInfoTest.c \

# C includes
C_INCLUDES =  \
-ILibraries/CMSIS/Include \
-ILibraries/CMSIS/Device/ST/STM32F4xx/Include \
-ILibraries/STM32F4xx_StdPeriph_Driver/inc \
-IUser/bsp/inc \
-IUser/bsp \
-IUser \
-IFreeRTOS/include \
-IFreeRTOS/portable/GCC/ARM_CM3

C_SOURCES= $(sort $(C_SOURCES2))

# LDSCRIPT = STM32F103C8Tx_FLASH.ld
# LDSCRIPT = STM32F103ZETx_FLASH.ld
# LDSCRIPT = STM32F407VETx_FLASH.ld
LDSCRIPT = STM32F407ZGTx_FLASH.ld

# ASM sources
ASM_SOURCES =  \
startup_stm32f407xx.s
# startup_stm32f407xx.s
# startup_stm32f103xe.s
# startup_stm32f103xb.s 

# CFLAGS= -mcpu=cortex-m3 -mthumb -DSTM32F103xB -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -DAPP_DEBUG=0 $(C_INCLUDES)  -Wall -fdata-sections -ffunction-sections -lc -lm -lnosys  -g -gdwarf-2 -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"
# CFLAGS= -mcpu=cortex-m3 -mthumb -DSTM32F103xE -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -DAPP_DEBUG=0 $(C_INCLUDES)  -Wall -fdata-sections -ffunction-sections -lc -lm -lnosys  -g -gdwarf-2 -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"
CFLAGS= -mcpu=cortex-m4 -mthumb -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DAPP_DEBUG=0 $(C_INCLUDES)  -Wall -fdata-sections -ffunction-sections -lc -lm -lnosys  -g -gdwarf-2 -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"
# STM32F40_41xxx,USE_STDPERIPH_DRIVER  in keil
# -DSTM32F407xx  in cubemx

# ASFLAGS= -mcpu=cortex-m3 -mthumb -DSTM32F103xB -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -DAPP_DEBUG=0 $(C_INCLUDES)  -Wall -fdata-sections -ffunction-sections -lc -lm -lnosys  -g -gdwarf-2   -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" 
# ASFLAGS= -mcpu=cortex-m3 -mthumb -DSTM32F103xE -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -DAPP_DEBUG=0 $(C_INCLUDES)  -Wall -fdata-sections -ffunction-sections -lc -lm -lnosys  -g -gdwarf-2   -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" 
ASFLAGS= -mcpu=cortex-m4 -mthumb -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -DAPP_DEBUG=0 $(C_INCLUDES)  -Wall -fdata-sections -ffunction-sections -lc -lm -lnosys  -g -gdwarf-2   -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" 

# LDFLAGS = -mcpu=cortex-m3  -mthumb -specs=nano.specs -T$(LDSCRIPT)  -lc -lm -lnosys -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LDFLAGS = -mcpu=cortex-m4  -mthumb -specs=nano.specs -T$(LDSCRIPT)  -lc -lm -lnosys -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
  
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

PREFIX = arm-none-eabi-
CC = $(BINPATH) $(PREFIX)gcc
AS = $(BINPATH) $(PREFIX)gcc -x assembler-with-cpp
CP = $(BINPATH) $(PREFIX)objcopy
AR = $(BINPATH) $(PREFIX)ar
SZ = $(BINPATH) $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

all:   $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf  $(BUILD_DIR)/$(TARGET).bin  $(BUILD_DIR)/$(TARGET).hex  
	 
$(BUILD_DIR)/%.o: %.c    
	$(CC) -c $(CFLAGS) $^ -o $@
	@echo Building $@
$(BUILD_DIR)/%.o:%.s   
	$(AS) -c $(CFLAGS) $^ -o $@
	
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@
	$(SZ) $@
	
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf 
	$(HEX) $^ $@ 
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf 
	$(BIN) $^ $@	
	
$(BUILD_DIR):
	mkdir $@
clean:
	-rm -fR .dep $(BUILD_DIR)
