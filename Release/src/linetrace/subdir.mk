################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/linetrace/linetrace.cpp 

OBJS += \
./src/linetrace/linetrace.o 

CPP_DEPS += \
./src/linetrace/linetrace.d 


# Each subdirectory must supply rules for building sources it contributes
src/linetrace/%.o: ../src/linetrace/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


