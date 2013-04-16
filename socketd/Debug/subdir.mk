################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../configparser.C \
../main.C \
../socketd.C \
../test.C 

OBJS += \
./configparser.o \
./main.o \
./socketd.o \
./test.o 

C_UPPER_DEPS += \
./configparser.d \
./main.d \
./socketd.d \
./test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../include/ -O0 -g3 -Wall -c -fmessage-length=0 --std=c++0x -fpie -pie -Wno-pmf-conversions -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


