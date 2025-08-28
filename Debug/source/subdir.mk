################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/demo_info.cpp \
../source/get_top_n.cpp \
../source/main.cpp \
../source/model.cpp \
../source/model_mobilenet_ops_npu.cpp \
../source/output_postproc.cpp 

C_SRCS += \
../source/image_decode_raw.c \
../source/image_load.c \
../source/mobilenet_tasks.c \
../source/perf_timer.c \
../source/scheduler.c \
../source/semihost_hardfault.c \
../source/task_utils.c \
../source/timer.c 

CPP_DEPS += \
./source/demo_info.d \
./source/get_top_n.d \
./source/main.d \
./source/model.d \
./source/model_mobilenet_ops_npu.d \
./source/output_postproc.d 

C_DEPS += \
./source/image_decode_raw.d \
./source/image_load.d \
./source/mobilenet_tasks.d \
./source/perf_timer.d \
./source/scheduler.d \
./source/semihost_hardfault.d \
./source/task_utils.d \
./source/timer.d 

OBJS += \
./source/demo_info.o \
./source/get_top_n.o \
./source/image_decode_raw.o \
./source/image_load.o \
./source/main.o \
./source/mobilenet_tasks.o \
./source/model.o \
./source/model_mobilenet_ops_npu.o \
./source/output_postproc.o \
./source/perf_timer.o \
./source/scheduler.o \
./source/semihost_hardfault.o \
./source/task_utils.o \
./source/timer.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=gnu++11 -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE_UART -DARM_MATH_CM33 -D__FPU_PRESENT=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DTF_LITE_STATIC_MEMORY -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -DSDK_OS_FREE_RTOS -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\drivers" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\CMSIS" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\CMSIS\m-profile" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\device" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\device\periph" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\component\lists" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities\str" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities\debug_console_lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\component\uart" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\flatbuffers\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\gemmlowp" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\kissfft" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\ruy" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\cmsis_nn" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\cmsis_nn\Include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\tensorflow\lite\micro\kernels\neutron" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\neutron\common\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\neutron\driver\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\template" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\template\ARM_CM33_3_priority_bits" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\source" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\board" -O0 -fno-common -g3 -gdwarf-4 -Wall -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -fno-rtti -fno-exceptions -imacros "C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__NEWLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE_UART -DARM_MATH_CM33 -D__FPU_PRESENT=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DTF_LITE_STATIC_MEMORY -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\drivers" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\CMSIS" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\CMSIS\m-profile" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\device" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\device\periph" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\component\lists" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities\str" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\utilities\debug_console_lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\component\uart" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\flatbuffers\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\gemmlowp" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\kissfft" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\ruy" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\cmsis_nn" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\cmsis_nn\Include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\tensorflow\lite\micro\kernels\neutron" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\neutron\common\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\eiq\tensorflow-lite\third_party\neutron\driver\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\include" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\template" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\freertos\freertos-kernel\template\ARM_CM33_3_priority_bits" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\source" -I"C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\board" -O0 -fno-common -g3 -gdwarf-4 -Wall -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\Domen\Documents\MCUXpressoIDE_24.12.148\workspace\custom_TaskModel_preempt\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/demo_info.d ./source/demo_info.o ./source/get_top_n.d ./source/get_top_n.o ./source/image_decode_raw.d ./source/image_decode_raw.o ./source/image_load.d ./source/image_load.o ./source/main.d ./source/main.o ./source/mobilenet_tasks.d ./source/mobilenet_tasks.o ./source/model.d ./source/model.o ./source/model_mobilenet_ops_npu.d ./source/model_mobilenet_ops_npu.o ./source/output_postproc.d ./source/output_postproc.o ./source/perf_timer.d ./source/perf_timer.o ./source/scheduler.d ./source/scheduler.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/task_utils.d ./source/task_utils.o ./source/timer.d ./source/timer.o

.PHONY: clean-source

