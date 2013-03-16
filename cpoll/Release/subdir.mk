################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../cpoll.C \
../sendfd.C \
../statemachines.C 

OBJS += \
./cpoll.o \
./sendfd.o \
./statemachines.o 

C_UPPER_DEPS += \
./cpoll.d \
./sendfd.d \
./statemachines.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.C
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I../../include -O3 -Wall -c -fmessage-length=0 --std=c++0x -Wno-pmf-conversions -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


