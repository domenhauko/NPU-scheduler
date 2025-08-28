################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/fsl_component_generic_list.c 

C_DEPS += \
./component/lists/fsl_component_generic_list.d 

OBJS += \
./component/lists/fsl_component_generic_list.o 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c component/lists/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__NEWLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE_UART -DARM_MATH_CM33 -D__FPU_PRESENT=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DTF_LITE_STATIC_MEMORY -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\drivers" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\CMSIS" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\CMSIS\m-profile" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\device" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\device\periph" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\component\lists" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities\str" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities\debug_console_lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\component\uart" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\flatbuffers\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\gemmlowp" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\kissfft" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\ruy" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\cmsis_nn" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\cmsis_nn\Include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\tensorflow\lite\micro\kernels\neutron" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\neutron\common\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\neutron\driver\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\template" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\template\ARM_CM33_3_priority_bits" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\source" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\board" -O0 -fno-common -g3 -gdwarf-4 -Wall -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-lists

clean-component-2f-lists:
	-$(RM) ./component/lists/fsl_component_generic_list.d ./component/lists/fsl_component_generic_list.o

.PHONY: clean-component-2f-lists

