################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.cpp 

CPP_DEPS += \
./eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.d 

OBJS += \
./eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.o 


# Each subdirectory must supply rules for building sources it contributes
eiq/tensorflow-lite/tensorflow/lite/micro/%.o: ../eiq/tensorflow-lite/tensorflow/lite/micro/%.cpp eiq/tensorflow-lite/tensorflow/lite/micro/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=gnu++11 -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE_UART -DARM_MATH_CM33 -D__FPU_PRESENT=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DTF_LITE_STATIC_MEMORY -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -DSDK_OS_FREE_RTOS -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\drivers" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\CMSIS" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\CMSIS\m-profile" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\device" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\device\periph" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\component\lists" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities\str" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities\debug_console_lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\component\uart" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\flatbuffers\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\gemmlowp" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\kissfft" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\ruy" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\cmsis_nn" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\cmsis_nn\Include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\tensorflow\lite\micro\kernels\neutron" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\neutron\common\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\neutron\driver\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\template" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\template\ARM_CM33_3_priority_bits" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\source" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\board" -O0 -fno-common -g3 -gdwarf-4 -Wall -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -fno-rtti -fno-exceptions -imacros "C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-eiq-2f-tensorflow-2d-lite-2f-tensorflow-2f-lite-2f-micro

clean-eiq-2f-tensorflow-2d-lite-2f-tensorflow-2f-lite-2f-micro:
	-$(RM) ./eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.d ./eiq/tensorflow-lite/tensorflow/lite/micro/debug_log.o

.PHONY: clean-eiq-2f-tensorflow-2d-lite-2f-tensorflow-2f-lite-2f-micro

