################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
fatfs/%.obj: ../fatfs/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7A8 --code_state=32 --float_support=VFPv3 -me -O3 --include_path="D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/include" --include_path="../../../include" --include_path="../../../include/hw" --include_path="../../../include/armv7a" --include_path="../../../include/armv7a/am335x" --include_path="../../../mmcsdlib/include" --include_path="D:/AM335X_StarterWare/project/FlixCNC/driver/include" --include_path="D:/AM335X_StarterWare/project/FlixCNC/graphics/include" --include_path="D:/AM335X_StarterWare/project/FlixCNC/fatfs/include" --include_path="D:/AM335X_StarterWare/project/FlixCNC/flixcnc/include" --include_path="D:/AM335X_StarterWare/project/FlixCNC/user/include" --include_path="D:/AM335X_StarterWare/project/FlixCNC/layout/include" --define=am3352 --symdebug:none --c99 --float_operations_allowed=all --printf_support=minimal --diag_warning=225 --display_error_number --neon --abi=eabi --preproc_with_compile --preproc_dependency="fatfs/$(basename $(<F)).d_raw" --obj_directory="fatfs" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


