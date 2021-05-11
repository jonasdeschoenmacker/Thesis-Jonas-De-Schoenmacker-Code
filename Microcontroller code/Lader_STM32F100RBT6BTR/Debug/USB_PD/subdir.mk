################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB_PD/STUB4500.c 

OBJS += \
./USB_PD/STUB4500.o 

C_DEPS += \
./USB_PD/STUB4500.d 


# Each subdirectory must supply rules for building sources it contributes
USB_PD/STUB4500.o: ../USB_PD/STUB4500.c USB_PD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F100xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"USB_PD/STUB4500.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

