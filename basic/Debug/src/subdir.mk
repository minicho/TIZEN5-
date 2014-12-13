################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AppResourceId.cpp \
../src/BasicApp.cpp \
../src/BasicAppEntry.cpp \
../src/BasicAppFrame.cpp \
../src/ButtonPanel.cpp \
../src/CalendarViewerForm.cpp \
../src/ChatForm.cpp \
../src/ChatMain.cpp \
../src/CreateEventForm.cpp \
../src/DeleteRecurringEventPopup.cpp \
../src/EditEventForm.cpp \
../src/EventDetailForm.cpp \
../src/EventListForm.cpp \
../src/MainForm.cpp \
../src/SelectCalendarPopup.cpp \
../src/SetRecurrenceForm.cpp \
../src/tabTestFormFactory.cpp \
../src/tabTestPanelFactory.cpp 

OBJS += \
./src/AppResourceId.o \
./src/BasicApp.o \
./src/BasicAppEntry.o \
./src/BasicAppFrame.o \
./src/ButtonPanel.o \
./src/CalendarViewerForm.o \
./src/ChatForm.o \
./src/ChatMain.o \
./src/CreateEventForm.o \
./src/DeleteRecurringEventPopup.o \
./src/EditEventForm.o \
./src/EventDetailForm.o \
./src/EventListForm.o \
./src/MainForm.o \
./src/SelectCalendarPopup.o \
./src/SetRecurrenceForm.o \
./src/tabTestFormFactory.o \
./src/tabTestPanelFactory.o 

CPP_DEPS += \
./src/AppResourceId.d \
./src/BasicApp.d \
./src/BasicAppEntry.d \
./src/BasicAppFrame.d \
./src/ButtonPanel.d \
./src/CalendarViewerForm.d \
./src/ChatForm.d \
./src/ChatMain.d \
./src/CreateEventForm.d \
./src/DeleteRecurringEventPopup.d \
./src/EditEventForm.d \
./src/EventDetailForm.d \
./src/EventListForm.d \
./src/MainForm.d \
./src/SelectCalendarPopup.d \
./src/SetRecurrenceForm.d \
./src/tabTestFormFactory.d \
./src/tabTestPanelFactory.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	arm-linux-gnueabi-g++.exe -I"pch" -D_DEBUG -I"C:\Users\ajou\workspace\basic\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fPIE --sysroot="C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/libxml2" -I"C:\tizen-sdk\library" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-device-2.2.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


