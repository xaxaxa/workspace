################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../config.C \
../main.C \
../test.C 

OBJS += \
./config.o \
./main.o \
./test.o 

C_UPPER_DEPS += \
./config.d \
./main.d \
./test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../include/ -O0 -g3 -Wall -c -fmessage-length=0 --std=c++0x -fpie -pie -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


