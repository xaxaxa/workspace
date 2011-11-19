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
<<<<<<< HEAD
	g++ -I"/home/xaxaxa/workspace/cplib/headers" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
=======
	g++ -I"/home/user1/workspace/cplib/headers" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
>>>>>>> ff921a1ddfc3fa60272a036ba466a64b53033796
	@echo 'Finished building: $<'
	@echo ' '


