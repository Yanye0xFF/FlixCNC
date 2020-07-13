################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./driver/crc16.obj" \
"./driver/i2c_port2.obj" \
"./driver/mcspi_port0.obj" \
"./driver/mcspi_port1.obj" \
"./driver/mmu_conf.obj" \
"./driver/network.obj" \
"./driver/sdcard_conf.obj" \
"./driver/st7735s.obj" \
"./fatfs/diskio.obj" \
"./fatfs/ff.obj" \
"./flixcnc/interpreter.obj" \
"./flixcnc/planner.obj" \
"./flixcnc/spindle.obj" \
"./flixcnc/stepper.obj" \
"./flixcnc/system.obj" \
"./flixcnc/utils.obj" \
"./graphics/color.obj" \
"./graphics/displayio.obj" \
"./graphics/frame.obj" \
"./graphics/imageview.obj" \
"./graphics/internal_font.obj" \
"./graphics/minifont.obj" \
"./graphics/progressbar.obj" \
"./graphics/textview.obj" \
"./layout/app_view.obj" \
"./layout/boot_view.obj" \
"./layout/conn_view.obj" \
"./layout/text.obj" \
"./user/bytestream.obj" \
"./user/comm_group.obj" \
"./user/context.obj" \
"./user/file_group.obj" \
"./user/main.obj" \
"./user/motion_group.obj" \
"./user/params_group.obj" \
"../app.cmd" \
$(GEN_CMDS__FLAG) \
-l"D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/lib/libc.a" \
-l"../../../library/vfp/mmcsd.lib" \
-l"../../../library/vfp/drivers.lib" \
-l"../../../library/vfp/system.lib" \
-l"../../../library/vfp/platform.lib" \
-l"../../../library/vfp/utils.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include driver/subdir_vars.mk
-include fatfs/subdir_vars.mk
-include flixcnc/subdir_vars.mk
-include graphics/subdir_vars.mk
-include layout/subdir_vars.mk
-include user/subdir_vars.mk
-include subdir_rules.mk
-include driver/subdir_rules.mk
-include fatfs/subdir_rules.mk
-include flixcnc/subdir_rules.mk
-include graphics/subdir_rules.mk
-include layout/subdir_rules.mk
-include user/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
../../../output/FlixCNC/Release/FlixCNC.out \

EXE_OUTPUTS__QUOTED += \
"..\..\..\output\FlixCNC\Release\FlixCNC.out" \

BIN_OUTPUTS += \
../../../output/FlixCNC/Release/FlixCNC.bin \

BIN_OUTPUTS__QUOTED += \
"..\..\..\output\FlixCNC\Release\FlixCNC.bin" \


# All Target
all: ../../../output/FlixCNC/Release/FlixCNC.out

# Tool invocations
../../../output/FlixCNC/Release/FlixCNC.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7A8 --code_state=32 --float_support=VFPv3 -me -O3 --define=am3352 --symdebug:none --c99 --float_operations_allowed=all --printf_support=minimal --diag_warning=225 --display_error_number --neon --abi=eabi -z -m"FlixCNC.map" --heap_size=0xC000000 --stack_size=0x80000 -i"D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/lib" -i"D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/include" --reread_libs --define=A8_CORE=1 --warn_sections --xml_link_info="../../../output/FlixCNC/Release/linkInfo.xml" --ram_model -o "../../../output/FlixCNC/Release/FlixCNC.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

../../../output/FlixCNC/Release/FlixCNC.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armhex"  -o "../../../output/FlixCNC/Release/FlixCNC.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "driver\crc16.obj" "driver\i2c_port2.obj" "driver\mcspi_port0.obj" "driver\mcspi_port1.obj" "driver\mmu_conf.obj" "driver\network.obj" "driver\sdcard_conf.obj" "driver\st7735s.obj" "fatfs\diskio.obj" "fatfs\ff.obj" "flixcnc\interpreter.obj" "flixcnc\planner.obj" "flixcnc\spindle.obj" "flixcnc\stepper.obj" "flixcnc\system.obj" "flixcnc\utils.obj" "graphics\color.obj" "graphics\displayio.obj" "graphics\frame.obj" "graphics\imageview.obj" "graphics\internal_font.obj" "graphics\minifont.obj" "graphics\progressbar.obj" "graphics\textview.obj" "layout\app_view.obj" "layout\boot_view.obj" "layout\conn_view.obj" "layout\text.obj" "user\bytestream.obj" "user\comm_group.obj" "user\context.obj" "user\file_group.obj" "user\main.obj" "user\motion_group.obj" "user\params_group.obj" 
	-$(RM) "driver\crc16.d" "driver\i2c_port2.d" "driver\mcspi_port0.d" "driver\mcspi_port1.d" "driver\mmu_conf.d" "driver\network.d" "driver\sdcard_conf.d" "driver\st7735s.d" "fatfs\diskio.d" "fatfs\ff.d" "flixcnc\interpreter.d" "flixcnc\planner.d" "flixcnc\spindle.d" "flixcnc\stepper.d" "flixcnc\system.d" "flixcnc\utils.d" "graphics\color.d" "graphics\displayio.d" "graphics\frame.d" "graphics\imageview.d" "graphics\internal_font.d" "graphics\minifont.d" "graphics\progressbar.d" "graphics\textview.d" "layout\app_view.d" "layout\boot_view.d" "layout\conn_view.d" "layout\text.d" "user\bytestream.d" "user\comm_group.d" "user\context.d" "user\file_group.d" "user\main.d" "user\motion_group.d" "user\params_group.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"D:/ccs930/ccs/utils/tiobj2bin/tiobj2bin.bat"  "../../../output/FlixCNC/Release/FlixCNC.out"  "../../../output/FlixCNC/Release/FlixCNC.bin"  "D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armofd.exe"  "D:/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armhex.exe"  "D:/ccs930/ccs/utils/tiobj2bin/mkhex4bin.exe" & "../../../tools/ti_image/tiimage.exe" "0x80000000" "NONE" "../../../output/FlixCNC/Release/FlixCNC.bin" "../../../output/FlixCNC/Release/FlixCNC_ti.bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

