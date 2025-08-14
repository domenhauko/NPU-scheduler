################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_ctimer.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpflexcomm.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_reset.c \
../drivers/fsl_spc.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_ctimer.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpflexcomm.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_reset.d \
./drivers/fsl_spc.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_ctimer.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpflexcomm.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_reset.o \
./drivers/fsl_spc.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__NEWLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE_UART -DARM_MATH_CM33 -D__FPU_PRESENT=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DTF_LITE_STATIC_MEMORY -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\drivers" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\CMSIS" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\CMSIS\m-profile" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\device" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\device\periph" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\utilities" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\component\lists" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\utilities\str" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\utilities\debug_console_lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\component\uart" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\third_party\flatbuffers\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\third_party\gemmlowp" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\third_party\kissfft" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\third_party\ruy" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\third_party\cmsis_nn" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\third_party\cmsis_nn\Include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\tensorflow\lite\micro\kernels\neutron" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\third_party\neutron\common\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\eiq\tensorflow-lite\third_party\neutron\driver\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\freertos\freertos-kernel\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\freertos\freertos-kernel\template" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\freertos\freertos-kernel\template\ARM_CM33_3_priority_bits" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\source" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\board" -O0 -fno-common -g3 -gdwarf-4 -Wall -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_ctimer.d ./drivers/fsl_ctimer.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpflexcomm.d ./drivers/fsl_lpflexcomm.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_spc.d ./drivers/fsl_spc.o

.PHONY: clean-drivers

