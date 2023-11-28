# Preempt REALTIME documentation: 

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
- Compile the code with "make" as usual
- Run the code with "sudo ./main". Sudo needs the be used otherwise problem occur. 

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
	
Setup the AD/DA board demo for analog input and output examples, better to try Waveshare device. 

	sudo apt-get install p7zip-full
	wget https://www.waveshare.com/w/upload/5/5e/High-Precision-AD-DA-Board-Code.7z
	7z x High-Precision-AD-DA-Board-Code.7z -r -o./High-Precision-AD-DA-Board-Code
	cd High-Precision-AD-DA-Board-Code/RaspberryPI/

For a basic rt application please refer to the: 

 - This is real time application code, it does not send signal to the analog output nor read. We will combine the two of them.
 - [Linux Foundation Wiki](https://wiki.linuxfoundation.org/realtime/documentation/howto/applications/start)
   Simple Example Code:

Install Cmake:
- This is required if you want to use Eigen function.

 CMake is an open-source, cross-platform tool for managing the build process of software projects. It generates native build files for various build systems, such as makefiles and Visual Studio projects. CMake can be used to build projects written in various programming languages, including C, C++, and Fortran.

Connect to Raspberry Pi via SSH. Run the following commands to update the package lists and install CMake:

```
sudo apt update
sudo apt install -y cmake
```

We can check version of CMake:
```
cmake --version
```

## **Testing CMake**

Create a new directory to store project files and navigate to this directory:

```
mkdir helloworld && cd helloworld
```

Create a `main.c` file:

```
nano main.c
```

Once the file is opened, add the following code:

helloworld/main.c

```
#include <stdio.h>

int main() {
    printf("Hello world\n");

return 0;
}
```

Create CMake configuration file called `CMakeLists.txt`:

```
nano CMakeLists.txt
```

Add the following content:

helloworld/CMakeLists.txt

```
cmake_minimum_required(VERSION 3.0)
project(hello C)

add_executable(hello main.c)
```

Recommended to create separate directory to store files that will be generated by CMake.

```
mkdir build && cd build
```

A project structure looks as follows:

```
helloworld/
    build/
    CMakeLists.txt
    main.c
```

In a build directory run the `cmake` command to generate build files using `CMakeLists.txt` file that located in parent directory. By default, CMake will generate build files for native build system. In our case it will be makefiles.

```
cmake ..
```

Once complete, we can use `ls` command to list files in a directory.

```
CMakeCache.txt  CMakeFiles  cmake_install.cmake  Makefile
```

As we can see, `Makefile` file has been generated. Now run the `make` command to build program:

```
make
```

Execute a program:

	sudo ./hello

Source: [Install CMake on Raspberry Pi | Lindevs](https://lindevs.com/install-cmake-on-raspberry-pi/)

- I suggest you use and edit my "CMakeTextList.txt". Its all in there.
- Realtime RPi should work right now.

  It seem with this way. We should have 2 analog outputs and 8 analog inputs. I tried for 2 analog outputs and 1 inputs and reached 1Khz. But it seems that without the calculations and such the system works mostly at 0.2µ.
  

Time Calculating

The time calculated using a function called "chrono::highresolutionclock()"

The library required is to "#include chrono"

In the beginning of the code write: 

	"auto start_time = std::chrono::high_resolution_clock::now();  "

At the end of the code write: 

	"auto end_time = std::chrono::high_resolution_clock::now(); "

This code below calculates time pass of the code run in 1 cycle: 

	"auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);  "
     
To print the code: 

	float(duration.count())/1000    


Also, 


