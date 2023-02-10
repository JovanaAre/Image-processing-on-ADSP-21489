################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ImageProcessing.c 

SRC_OBJS += \
./src/ImageProcessing.doj 

C_DEPS += \
./src/ImageProcessing.d 


# Each subdirectory must supply rules for building sources it contributes
src/ImageProcessing.doj: ../src/ImageProcessing.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="ImageProcessing" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -DDO_CYCLE_COUNTS -D_DEBUG @includes-ea19019b202691c110aeb50b3b2d149a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/ImageProcessing.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


