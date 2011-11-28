################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../tmp.o 

CPP_SRCS += \
../CacheManager.cpp \
../IStorage.cpp \
../StorageManager.cpp 

CXX_SRCS += \
../main.cxx 

OBJS += \
./CacheManager.o \
./IStorage.o \
./StorageManager.o \
./main.o 

CPP_DEPS += \
./CacheManager.d \
./IStorage.d \
./StorageManager.d 

CXX_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/s-wang_cary/local/include -O0 -g3 -Wall -c -fmessage-length=0 -Wno-pmf-conversions --std=gnu++0x -D_FILE_OFFSET_BITS=64 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/s-wang_cary/local/include -O0 -g3 -Wall -c -fmessage-length=0 -Wno-pmf-conversions --std=gnu++0x -D_FILE_OFFSET_BITS=64 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


