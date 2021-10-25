################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/XCP_Communication/UDP_IP.c \
../src/XCP_Communication/xcp_eth_init.c 

OBJS += \
./src/XCP_Communication/UDP_IP.o \
./src/XCP_Communication/xcp_eth_init.o 

C_DEPS += \
./src/XCP_Communication/UDP_IP.d \
./src/XCP_Communication/xcp_eth_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/XCP_Communication/%.o: ../src/XCP_Communication/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"C:\UTK_Project\Daimler_Project\XCP\XCP.sdk\xcp\src\XCP_Basic_Driver" -I"C:\UTK_Project\Daimler_Project\XCP\XCP.sdk\xcp\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../freertos10_xilinx_bsp_0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


