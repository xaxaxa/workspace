################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../cplib/asyncfile.cpp \
../cplib/asyncsock.cpp \
../cplib/cplib.cpp 

OBJS += \
./cplib/asyncfile.o \
./cplib/asyncsock.o \
./cplib/cplib.o 

CPP_DEPS += \
./cplib/asyncfile.d \
./cplib/asyncsock.d \
./cplib/cplib.d 


# Each subdirectory must supply rules for building sources it contributes
cplib/%.o: ../cplib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/s-wang_cary/local/include -O0 -g3 -Wall -c -fmessage-length=0 -Wno-pmf-conversions --std=gnu++0x -D_FILE_OFFSET_BITS=64 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


