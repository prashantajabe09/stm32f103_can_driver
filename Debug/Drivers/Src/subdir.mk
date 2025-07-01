################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/CAN.c \
../Drivers/Src/Clk.c \
../Drivers/Src/GPIO.c \
../Drivers/Src/System.c \
../Drivers/Src/UART.c \
../Drivers/Src/common.c 

OBJS += \
./Drivers/Src/CAN.o \
./Drivers/Src/Clk.o \
./Drivers/Src/GPIO.o \
./Drivers/Src/System.o \
./Drivers/Src/UART.o \
./Drivers/Src/common.o 

C_DEPS += \
./Drivers/Src/CAN.d \
./Drivers/Src/Clk.d \
./Drivers/Src/GPIO.d \
./Drivers/Src/System.d \
./Drivers/Src/UART.d \
./Drivers/Src/common.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su Drivers/Src/%.cyclo: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"C:/Users/Propix/STM32CubeIDE/workspace_1.9.0/STM32F103_Peripheral_Driver/Drivers/Inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/CAN.cyclo ./Drivers/Src/CAN.d ./Drivers/Src/CAN.o ./Drivers/Src/CAN.su ./Drivers/Src/Clk.cyclo ./Drivers/Src/Clk.d ./Drivers/Src/Clk.o ./Drivers/Src/Clk.su ./Drivers/Src/GPIO.cyclo ./Drivers/Src/GPIO.d ./Drivers/Src/GPIO.o ./Drivers/Src/GPIO.su ./Drivers/Src/System.cyclo ./Drivers/Src/System.d ./Drivers/Src/System.o ./Drivers/Src/System.su ./Drivers/Src/UART.cyclo ./Drivers/Src/UART.d ./Drivers/Src/UART.o ./Drivers/Src/UART.su ./Drivers/Src/common.cyclo ./Drivers/Src/common.d ./Drivers/Src/common.o ./Drivers/Src/common.su

.PHONY: clean-Drivers-2f-Src

