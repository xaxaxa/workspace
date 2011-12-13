################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../JoinStream.cpp \
../asdfghjkl.cpp \
../main.cpp \
../socketmux.cpp 

OBJS += \
./JoinStream.o \
./asdfghjkl.o \
./main.o \
./socketmux.o 

CPP_DEPS += \
./JoinStream.d \
./asdfghjkl.d \
./main.d \
./socketmux.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/s-wang_cary/workspace/cplib" -I"/home/s-wang_cary/workspace/cplib/headers" -O0 -g3 -p -pg -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


