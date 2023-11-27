# Preemp REALTIME documentation: 

This document is a guide to using PREEMPT_RT, which is a patch for the Linux kernel that provides real-time capabilities. In particular, we will be discussing the use of PREEMPT_RT on a Raspberry Pi system, along with the integration of the Waveshare AD/DA converter module. The document will provide an overview of the code used to implement a real-time periodic task, along with explanations and analysis of the various functions used in the code. 

Waveshare AD/DA converter module has DAC8532 chip is for to convert digital signals to analog. This is for the controlling the motor. The DAC8532 is a high-precision digital-to-analog converter that can be used to generate analog signals with a resolution of up to 16 bits. It is commonly used in applications where high accuracy and stability are required, such as in laboratory equipment and test and measurement systems. To use the DAC8532, it must first be connected to a microcontroller or other control device, and the appropriate control signals must be sent to the device to configure its operation and generate analog output signals. This can be done using a variety of programming languages and development environments, depending on the specific requirements of the application.

ADS1256 is used for to convert analog input to digital for reading encoder data from the motor. 

ADS1256 is an analog to digital converter (ADC) that can be used to convert analog signals to digital signals. This is useful for reading sensor data or other signals that are typically analog in nature. In the context of this document, the ADS1256 is used to read encoder data from a motor.

https://wiki.linuxfoundation.org/realtime/documentation/howto/applications/start

TL: DR 

- Install RT kernel to RPi
- Install CMake
- Instal BCM2835
- Realtime Codes are from the linux wiki, Analog output input codes are from waveshare website “example codes”.
- Use the code in the files.

EDIT THIS: Guide for the code provided: 
(This guide is for the RPİ3 with all the kernels have been set.)

- Download the files.
- Edit CMakeTextList.txt file's include directory to your include directory.
- Terminal enter the /main/build and write : > cmake..
- For variables I used roboleg_func.hpp
- You do not touch main.cpp file.
- The realtime loop code is in roboleg_func.cpp, to adjust sampling frequency edit : simple_cyclic_task()

Installing Guide:

# Install RT Kernel
-[Lemariva - Preempt RT Patching](https://lemariva.com/blog/2018/07/raspberry-pi-preempt-rt-patching-tutorial-for-kernel-4-14-y)  - Do according to tutorial.
-[Lemariva - Preempt RT Patching RPi4](https://lemariva.com/blog/2019/09/raspberry-pi-4b-preempt-rt-kernel-419y-performance-test) - Raspberry Pi 4b

Performance:
- Everytime you open the RPi enter the command below.
  - sudo -Es
  - echo performance | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

Install BCM2835 libraries: (For to use gpio pins of the RPi) [1]

	#Open the Raspberry Pi terminal and run the following command
	wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
	tar zxvf bcm2835-1.71.tar.gz
	cd bcm2835-1.71/
	sudo ./configure && sudo make && sudo make check && sudo make install
	# For more, you can refer to the official website at: http://www.airspayce.com/mikem/bcm2835/
	

