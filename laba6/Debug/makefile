################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := E:/CCS/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./CTS_HAL.obj" \
"./CTS_Layer.obj" \
"./HAL_AppUart.obj" \
"./HAL_Board.obj" \
"./HAL_Cma3000.obj" \
"./HAL_Dogs102x6.obj" \
"./HAL_FLASH.obj" \
"./HAL_PMAP.obj" \
"./HAL_PMM.obj" \
"./HAL_SDCard.obj" \
"./HAL_TLV.obj" \
"./HAL_UCS.obj" \
"./main.obj" \
"./screen_utils.obj" \
"./structure.obj" \
"./utils.obj" \
"../lnk_msp430f5529.cmd" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
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
laba6.out \

EXE_OUTPUTS__QUOTED += \
"laba6.out" \


# All Target
all: laba6.out

# Tool invocations
laba6.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"E:/CCS/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/bin/cl430" -vmspx --abi=eabi --advice:power=all -g --relaxed_ansi --define=__MSP430F5529__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal -z -m"laba6.map" --heap_size=160 --stack_size=160 --use_hw_mpy=F5 -i"E:/CCS/ccs/ccs_base/msp430/include" -i"C:/Users/virot/workspace_v10/laba6/libs" -i"E:/CCS/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/lib" -i"E:/CCS/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/include" -i"E:/CCS/ccs/ccs_base/msp430/lib" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="laba6_linkInfo.xml" --rom_model -o "laba6.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "CTS_HAL.obj" "CTS_Layer.obj" "HAL_AppUart.obj" "HAL_Board.obj" "HAL_Cma3000.obj" "HAL_Dogs102x6.obj" "HAL_FLASH.obj" "HAL_PMAP.obj" "HAL_PMM.obj" "HAL_SDCard.obj" "HAL_TLV.obj" "HAL_UCS.obj" "main.obj" "screen_utils.obj" "structure.obj" "utils.obj" 
	-$(RM) "CTS_HAL.d" "CTS_Layer.d" "HAL_AppUart.d" "HAL_Board.d" "HAL_Cma3000.d" "HAL_Dogs102x6.d" "HAL_FLASH.d" "HAL_PMAP.d" "HAL_PMM.d" "HAL_SDCard.d" "HAL_TLV.d" "HAL_UCS.d" "main.d" "screen_utils.d" "structure.d" "utils.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

