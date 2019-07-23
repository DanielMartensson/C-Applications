################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/actions.c \
../src/cmd_def.c \
../src/commands.c \
../src/main.c \
../src/uart.c 

OBJS += \
./src/actions.o \
./src/cmd_def.o \
./src/commands.o \
./src/main.o \
./src/uart.o 

C_DEPS += \
./src/actions.d \
./src/cmd_def.d \
./src/commands.d \
./src/main.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


