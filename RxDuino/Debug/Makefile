################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../RxDuino.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
RxDuino.o


OBJS_AS_ARGS +=  \
RxDuino.o


C_DEPS +=  \
RxDuino.d


C_DEPS_AS_ARGS +=  \
RxDuino.d


OUTPUT_FILE_PATH +=RxDuino.elf

OUTPUT_FILE_PATH_AS_ARGS +=RxDuino.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

# AVR32/GNU C Compiler



./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C++ Compiler : (AVR_8_bit_GNU_Toolchain_3.4.0_663) 4.6.2
	$(QUOTE)C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -I"C:\Users\Corei7\Documents\Atmel Studio\Headers\Public" -I"C:\Users\Corei7\Documents\Atmel Studio\Headers\Arduino" -I"C:\Users\Corei7\Documents\Atmel Studio\Headers\ArduinoCore" -I"C:\Users\Corei7\Downloads\arduino-1.0.3-windows\arduino-1.0.3\libraries\MCP2515" -I"../../RxDuinoLibs/include"  -Os -fpack-struct -fshort-enums -g2 -Wall -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -mmcu=atmega328p   -o"$@" "$<" 
	@echo Finished building: $<



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : (AVR_8_bit_GNU_Toolchain_3.4.0_663) 4.6.2
	$(QUOTE)C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="RxDuino.map" -Wl,--start-group -Wl,-lcore -Wl,-lRxduinoLibs  -Wl,--end-group -Wl,-L"../../../../Atmel Studio/Libs/Arduino" -Wl,-L"../../../../Atmel Studio/Libs/ArduinoCore" -Wl,-L"../../../../Atmel Studio/Libs/Public" -Wl,-L"../../RxDuinoLibs/Debug"  -Wl,--gc-sections -Wl,-lcore  -Wl,-lm  -mmcu=atmega328p  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "RxDuino.elf" "RxDuino.hex"
	"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "RxDuino.elf" "RxDuino.eep" || exit 0
	"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-objdump.exe" -h -S "RxDuino.elf" > "RxDuino.lss"
	"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-size.exe" "RxDuino.elf"
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) 
	rm -rf "RxDuino.elf" "RxDuino.a" "RxDuino.hex" "RxDuino.lss" "RxDuino.eep" "RxDuino.map"
	