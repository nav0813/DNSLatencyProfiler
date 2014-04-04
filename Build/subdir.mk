################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DBHandler.cpp \
../DNSLatencyProfiler.cpp \
../Executor.cpp \
../QueryExecutor.cpp \
../QueryResult.cpp \
../TimeTrigger.cpp \
../Trigger.cpp \
../Utils.cpp \
../main.cpp 

OBJS += \
./DBHandler.o \
./DNSLatencyProfiler.o \
./Executor.o \
./QueryExecutor.o \
./QueryResult.o \
./TimeTrigger.o \
./Trigger.o \
./Utils.o \
./main.o 

CPP_DEPS += \
./DBHandler.d \
./DNSLatencyProfiler.d \
./Executor.d \
./QueryExecutor.d \
./QueryResult.d \
./TimeTrigger.d \
./Trigger.d \
./Utils.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/include/mysql -I/usr/local/include/mysql++ -O0 -g3 -Wall -std=c++11 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


