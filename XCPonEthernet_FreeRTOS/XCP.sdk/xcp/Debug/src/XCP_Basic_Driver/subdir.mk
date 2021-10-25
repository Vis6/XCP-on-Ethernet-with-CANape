################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/XCP_Basic_Driver/XcpBasic.c \
../src/XCP_Basic_Driver/_xcp_appl.c \
../src/XCP_Basic_Driver/xcp_par.c 

OBJS += \
./src/XCP_Basic_Driver/XcpBasic.o \
./src/XCP_Basic_Driver/_xcp_appl.o \
./src/XCP_Basic_Driver/xcp_par.o 

C_DEPS += \
./src/XCP_Basic_Driver/XcpBasic.d \
./src/XCP_Basic_Driver/_xcp_appl.d \
./src/XCP_Basic_Driver/xcp_par.d 


# Each subdirectory must supply rules for building sources it contributes
src/XCP_Basic_Driver/%.o: ../src/XCP_Basic_Driver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"C:\Users\xzhan134\Documents\GitHub\XCP-on-Ethernet-with-CANape\XCPonEthernet_FreeRTOS\XCP.sdk\xcp\src\XCP_Basic_Driver" -I"C:\Users\xzhan134\Documents\GitHub\XCP-on-Ethernet-with-CANape\XCPonEthernet_FreeRTOS\XCP.sdk\xcp\src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../freertos10_xilinx_bsp_0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


