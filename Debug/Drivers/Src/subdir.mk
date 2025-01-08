################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/Clk.c \
../Drivers/Src/DMA.c \
../Drivers/Src/GPIO.c \
../Drivers/Src/I2C.c \
../Drivers/Src/System.c \
../Drivers/Src/Timer.c \
../Drivers/Src/UART.c \
../Drivers/Src/common.c \
../Drivers/Src/iwdg.c \
../Drivers/Src/spi.c 

OBJS += \
./Drivers/Src/Clk.o \
./Drivers/Src/DMA.o \
./Drivers/Src/GPIO.o \
./Drivers/Src/I2C.o \
./Drivers/Src/System.o \
./Drivers/Src/Timer.o \
./Drivers/Src/UART.o \
./Drivers/Src/common.o \
./Drivers/Src/iwdg.o \
./Drivers/Src/spi.o 

C_DEPS += \
./Drivers/Src/Clk.d \
./Drivers/Src/DMA.d \
./Drivers/Src/GPIO.d \
./Drivers/Src/I2C.d \
./Drivers/Src/System.d \
./Drivers/Src/Timer.d \
./Drivers/Src/UART.d \
./Drivers/Src/common.d \
./Drivers/Src/iwdg.d \
./Drivers/Src/spi.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"C:/Users/Propix/STM32CubeIDE/workspace_1.9.0/001_Pulse_Generating_Card_20241129/Drivers/Inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/Clk.d ./Drivers/Src/Clk.o ./Drivers/Src/Clk.su ./Drivers/Src/DMA.d ./Drivers/Src/DMA.o ./Drivers/Src/DMA.su ./Drivers/Src/GPIO.d ./Drivers/Src/GPIO.o ./Drivers/Src/GPIO.su ./Drivers/Src/I2C.d ./Drivers/Src/I2C.o ./Drivers/Src/I2C.su ./Drivers/Src/System.d ./Drivers/Src/System.o ./Drivers/Src/System.su ./Drivers/Src/Timer.d ./Drivers/Src/Timer.o ./Drivers/Src/Timer.su ./Drivers/Src/UART.d ./Drivers/Src/UART.o ./Drivers/Src/UART.su ./Drivers/Src/common.d ./Drivers/Src/common.o ./Drivers/Src/common.su ./Drivers/Src/iwdg.d ./Drivers/Src/iwdg.o ./Drivers/Src/iwdg.su ./Drivers/Src/spi.d ./Drivers/Src/spi.o ./Drivers/Src/spi.su

.PHONY: clean-Drivers-2f-Src

