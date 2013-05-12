################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../common.C \
../cppsp_cpoll.C \
../page.C \
../stringutils.C 

OBJS += \
./common.o \
./cppsp_cpoll.o \
./page.o \
./stringutils.o 

C_UPPER_DEPS += \
./common.d \
./cppsp_cpoll.d \
./page.d \
./stringutils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../include/ -O0 -g3 -Wall -c -fmessage-length=0 --std=c++0x -Wno-pmf-conversions -fno-omit-frame-pointer -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


