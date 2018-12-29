# ST Visual Debugger Generated MAKE File, based on bottler4.stp

ifeq ($(CFG), )
CFG=Debug
$(warning ***No configuration specified. Defaulting to $(CFG)***)
endif

ToolsetRoot=C:\PROGRA~2\COSMIC\FSE_CO~1\CXSTM8
ToolsetBin=C:\Program Files (x86)\COSMIC\FSE_Compilers\CXSTM8
ToolsetInc=C:\Program Files (x86)\COSMIC\FSE_Compilers\CXSTM8\Hstm8
ToolsetLib=C:\Program Files (x86)\COSMIC\FSE_Compilers\CXSTM8\Lib
ToolsetIncOpts=-i"C:\Program Files (x86)\COSMIC\FSE_Compilers\CXSTM8\Hstm8" 
ToolsetLibOpts=-l"C:\Program Files (x86)\COSMIC\FSE_Compilers\CXSTM8\Lib" 
ObjectExt=o
OutputExt=elf
InputName=$(basename $(notdir $<))


# 
# Debug
# 
ifeq "$(CFG)" "Debug"


OutputPath=Debug
ProjectSFile=bottler4
TargetSName=$(ProjectSFile)
TargetFName=$(ProjectSFile).elf
IntermPath=$(dir $@)
CFLAGS_PRJ=$(ToolsetBin)\cxstm8  +mods0 +debug -pxp -no -pp -l $(ToolsetIncOpts) -cl$(IntermPath:%\=%) -co$(IntermPath:%\=%) $<
ASMFLAGS_PRJ=$(ToolsetBin)\castm8  -xx -l $(ToolsetIncOpts) -o$(IntermPath)$(InputName).$(ObjectExt) $<

all : $(OutputPath) $(ProjectSFile).elf

$(OutputPath) : 
	if not exist $(OutputPath)/ mkdir $(OutputPath)

Debug\bottler4_stm8s105_main.$(ObjectExt) : bottler4_stm8s105_main.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdio.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h scheduler.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\string.h eep.h i2c.h uart.h lcd_i2c.h ..\..\..\..\..\delay.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Debug\delay.$(ObjectExt) : ..\..\..\..\..\delay.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h ..\..\..\..\..\delay.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Debug\eep.$(ObjectExt) : eep.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h eep.h stdint.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Debug\i2c.$(ObjectExt) : i2c.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h i2c.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h bottler4_stm8s105_main.h ..\..\..\..\..\delay.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Debug\lcd_i2c.$(ObjectExt) : lcd_i2c.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h lcd_i2c.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h ..\..\..\..\..\delay.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H i2c.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Debug\rev_nr.$(ObjectExt) : rev_nr.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Debug\scheduler.$(ObjectExt) : scheduler.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h scheduler.h stdint.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\string.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdio.h ..\..\..\..\..\delay.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H uart.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Debug\stm8_interrupt_vector.$(ObjectExt) : stm8_interrupt_vector.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Debug\uart.$(ObjectExt) : uart.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\ctype.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdlib.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdio.h uart.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h i2c.h scheduler.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\string.h ring_buffer.h ..\..\..\..\..\delay.h lcd_i2c.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

$(ProjectSFile).elf :  $(OutputPath)\bottler4_stm8s105_main.o $(OutputPath)\delay.o $(OutputPath)\eep.o $(OutputPath)\i2c.o $(OutputPath)\lcd_i2c.o $(OutputPath)\rev_nr.o $(OutputPath)\scheduler.o $(OutputPath)\stm8_interrupt_vector.o $(OutputPath)\uart.o $(OutputPath)\bottler4.lkf
	$(ToolsetBin)\clnk  $(ToolsetLibOpts) -o $(OutputPath)\$(TargetSName).sm8 -m$(OutputPath)\$(TargetSName).map $(OutputPath)\$(TargetSName).lkf 
	$(ToolsetBin)\cvdwarf  $(OutputPath)\$(TargetSName).sm8

	$(ToolsetBin)\chex  -fi -o $(OutputPath)\$(TargetSName).hex $(OutputPath)\$(TargetSName).sm8
clean : 
	-@erase $(OutputPath)\bottler4_stm8s105_main.o
	-@erase $(OutputPath)\delay.o
	-@erase $(OutputPath)\eep.o
	-@erase $(OutputPath)\i2c.o
	-@erase $(OutputPath)\lcd_i2c.o
	-@erase $(OutputPath)\rev_nr.o
	-@erase $(OutputPath)\scheduler.o
	-@erase $(OutputPath)\stm8_interrupt_vector.o
	-@erase $(OutputPath)\uart.o
	-@erase $(OutputPath)\bottler4.elf
	-@erase $(OutputPath)\bottler4.elf
	-@erase $(OutputPath)\bottler4.map
	-@erase $(OutputPath)\bottler4_stm8s105_main.ls
	-@erase $(OutputPath)\delay.ls
	-@erase $(OutputPath)\eep.ls
	-@erase $(OutputPath)\i2c.ls
	-@erase $(OutputPath)\lcd_i2c.ls
	-@erase $(OutputPath)\rev_nr.ls
	-@erase $(OutputPath)\scheduler.ls
	-@erase $(OutputPath)\stm8_interrupt_vector.ls
	-@erase $(OutputPath)\uart.ls
endif

# 
# Release
# 
ifeq "$(CFG)" "Release"


OutputPath=Release
ProjectSFile=bottler4
TargetSName=bottler4
TargetFName=bottler4.elf
IntermPath=$(dir $@)
CFLAGS_PRJ=$(ToolsetBin)\cxstm8  +mods0 -pp $(ToolsetIncOpts) -cl$(IntermPath:%\=%) -co$(IntermPath:%\=%) $< 
ASMFLAGS_PRJ=$(ToolsetBin)\castm8  $(ToolsetIncOpts) -o$(IntermPath)$(InputName).$(ObjectExt) $<

all : $(OutputPath) bottler4.elf

$(OutputPath) : 
	if not exist $(OutputPath)/ mkdir $(OutputPath)

Release\bottler4_stm8s105_main.$(ObjectExt) : bottler4_stm8s105_main.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdio.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h scheduler.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\string.h eep.h i2c.h uart.h lcd_i2c.h ..\..\..\..\..\delay.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Release\delay.$(ObjectExt) : ..\..\..\..\..\delay.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h ..\..\..\..\..\delay.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Release\eep.$(ObjectExt) : eep.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h eep.h stdint.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Release\i2c.$(ObjectExt) : i2c.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h i2c.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h bottler4_stm8s105_main.h ..\..\..\..\..\delay.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Release\lcd_i2c.$(ObjectExt) : lcd_i2c.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h lcd_i2c.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h ..\..\..\..\..\delay.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H i2c.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Release\rev_nr.$(ObjectExt) : rev_nr.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Release\scheduler.$(ObjectExt) : scheduler.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h scheduler.h stdint.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\string.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdio.h ..\..\..\..\..\delay.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H uart.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Release\stm8_interrupt_vector.$(ObjectExt) : stm8_interrupt_vector.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

Release\uart.$(ObjectExt) : uart.c c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\mods0.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\ctype.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdlib.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdio.h uart.h bottler4_stm8s105_main.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\IOSTM8~4.H c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\stdbool.h stdint.h i2c.h scheduler.h c:\PROGRA~2\cosmic\FSE_CO~1\cxstm8\hstm8\string.h ring_buffer.h ..\..\..\..\..\delay.h lcd_i2c.h 
	@if not exist $(dir $@)  mkdir $(dir $@)
	$(CFLAGS_PRJ)

bottler4.elf :  $(OutputPath)\bottler4_stm8s105_main.o $(OutputPath)\delay.o $(OutputPath)\eep.o $(OutputPath)\i2c.o $(OutputPath)\lcd_i2c.o $(OutputPath)\rev_nr.o $(OutputPath)\scheduler.o $(OutputPath)\stm8_interrupt_vector.o $(OutputPath)\uart.o $(OutputPath)\bottler4.lkf
	$(ToolsetBin)\clnk  $(ToolsetLibOpts) -o $(OutputPath)\$(TargetSName).sm8 $(OutputPath)\$(TargetSName).lkf 
	$(ToolsetBin)\cvdwarf  $(OutputPath)\$(TargetSName).sm8 

	$(ToolsetBin)\chex  -o $(OutputPath)\$(TargetSName).s19 $(OutputPath)\$(TargetSName).sm8
clean : 
	-@erase $(OutputPath)\bottler4_stm8s105_main.o
	-@erase $(OutputPath)\delay.o
	-@erase $(OutputPath)\eep.o
	-@erase $(OutputPath)\i2c.o
	-@erase $(OutputPath)\lcd_i2c.o
	-@erase $(OutputPath)\rev_nr.o
	-@erase $(OutputPath)\scheduler.o
	-@erase $(OutputPath)\stm8_interrupt_vector.o
	-@erase $(OutputPath)\uart.o
	-@erase $(OutputPath)\bottler4.elf
	-@erase $(OutputPath)\bottler4.map
	-@erase $(OutputPath)\bottler4.st7
	-@erase $(OutputPath)\bottler4.s19
	-@erase $(OutputPath)\bottler4_stm8s105_main.ls
	-@erase $(OutputPath)\delay.ls
	-@erase $(OutputPath)\eep.ls
	-@erase $(OutputPath)\i2c.ls
	-@erase $(OutputPath)\lcd_i2c.ls
	-@erase $(OutputPath)\rev_nr.ls
	-@erase $(OutputPath)\scheduler.ls
	-@erase $(OutputPath)\stm8_interrupt_vector.ls
	-@erase $(OutputPath)\uart.ls
endif
