################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
..\Aquarium.cpp \
..\util.cpp 

LINK_OBJ += \
.\Aquarium.cpp.o \
.\util.cpp.o 

CPP_DEPS += \
.\Aquarium.cpp.d \
.\util.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
Aquarium.cpp.o: ../Aquarium.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\EclipseCPPNeon\arduinoPlugin\tools\arduino\avr-gcc\4.8.1-arduino5/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR   -I"C:\EclipseCPPNeon\arduinoPlugin\packages\arduino\hardware\avr\1.6.5\cores\arduino" -I"C:\EclipseCPPNeon\arduinoPlugin\packages\arduino\hardware\avr\1.6.5\variants\eightanaloginputs" -I"D:\HomeAutomation\ArduinoLibs\DallasTemperature" -I"D:\HomeAutomation\ArduinoLibs\PubSubClient" -I"D:\HomeAutomation\ArduinoLibs\OneWire" -I"D:\HomeAutomation\ArduinoLibs\UIPEthernet" -I"D:\HomeAutomation\ArduinoLibs\UIPEthernet\utility" -I"D:\HomeAutomation\ArduinoLibs\Timer" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<" -o "$@"  -Wall
	@echo 'Finished building: $<'
	@echo ' '

util.cpp.o: ../util.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\EclipseCPPNeon\arduinoPlugin\tools\arduino\avr-gcc\4.8.1-arduino5/bin/avr-g++" -c -g -Os -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR   -I"C:\EclipseCPPNeon\arduinoPlugin\packages\arduino\hardware\avr\1.6.5\cores\arduino" -I"C:\EclipseCPPNeon\arduinoPlugin\packages\arduino\hardware\avr\1.6.5\variants\eightanaloginputs" -I"D:\HomeAutomation\ArduinoLibs\DallasTemperature" -I"D:\HomeAutomation\ArduinoLibs\PubSubClient" -I"D:\HomeAutomation\ArduinoLibs\OneWire" -I"D:\HomeAutomation\ArduinoLibs\UIPEthernet" -I"D:\HomeAutomation\ArduinoLibs\UIPEthernet\utility" -I"D:\HomeAutomation\ArduinoLibs\Timer" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<" -o "$@"  -Wall
	@echo 'Finished building: $<'
	@echo ' '


