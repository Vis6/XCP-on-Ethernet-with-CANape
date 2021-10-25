################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/PL_LED_Init/led_flash_task.c 

OBJS += \
./src/PL_LED_Init/led_flash_task.o 

C_DEPS += \
./src/PL_LED_Init/led_flash_task.d 


# Each subdirectory must supply rules for building sources it contributes
src/PL_LED_Init/%.o: ../src/PL_LED_Init/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"C:\UTK_Project\Daimler_Project\XCP\XCP.sdk\xcp\src\XCP_Basic_Driver" -I"C:\UTK_Project\Daimler_Project\XCP\XCP.sdk\xcp\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../freertos10_xilinx_bsp_0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


