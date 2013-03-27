################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../DNSServer.C \
../PacketQueue.C \
../joinstream.C \
../main.C \
../socks5.C 

OBJS += \
./DNSServer.o \
./PacketQueue.o \
./joinstream.o \
./main.o \
./socks5.o 

C_UPPER_DEPS += \
./DNSServer.d \
./PacketQueue.d \
./joinstream.d \
./main.d \
./socks5.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../include/ -O3 -Wall -c -fmessage-length=0 --std=c++0x -Wno-pmf-conversions -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


