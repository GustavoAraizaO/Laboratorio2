################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/source/croutine.c \
../freertos/source/event_groups.c \
../freertos/source/list.c \
../freertos/source/queue.c \
../freertos/source/tasks.c \
../freertos/source/timers.c 

OBJS += \
./freertos/source/croutine.o \
./freertos/source/event_groups.o \
./freertos/source/list.o \
./freertos/source/queue.o \
./freertos/source/tasks.o \
./freertos/source/timers.o 

C_DEPS += \
./freertos/source/croutine.d \
./freertos/source/event_groups.d \
./freertos/source/list.d \
./freertos/source/queue.d \
./freertos/source/tasks.d \
./freertos/source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/source/%.o: ../freertos/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -DUSB_STACK_FREERTOS -D_DEBUG=1 -DFSL_RTOS_FREE_RTOS -DCR_INTEGER_PRINTF -DUSB_STACK_FREERTOS_HEAP_SIZE=32768 -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\board" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\source" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\usb\device\source\khci" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\usb\include" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\osa" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\drivers" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\CMSIS" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\freertos\include" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\freertos\portable" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\utilities" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\startup" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\sources" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\usb\device\class\hid" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\usb\device\class" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\usb\device\source" -I"C:\Projects_2\Laboratorio2_Mouse_keryboard\usb\device\include" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


