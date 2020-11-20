################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gpio/gpio.cpp 

OBJS += \
./src/gpio/gpio.o 

CPP_DEPS += \
./src/gpio/gpio.d 


# Each subdirectory must supply rules for building sources it contributes
src/gpio/%.o: ../src/gpio/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


