################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../cpoll.o \
../rgc.o 

C_UPPER_SRCS += \
../cpoll.C 

OBJS += \
./cpoll.o 

C_UPPER_DEPS += \
./cpoll.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.C
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/home/xaxaxa/workspace/include -O0 -g3 -Wall -c -fmessage-length=0 --std=c++0x -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

