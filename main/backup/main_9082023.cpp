#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include "stdio.h"
#include <string.h>
#include <chrono>

#include "pthread.h"
#include <cstddef>
#include <limits.h>
#include <sched.h>
#include <sys/mman.h>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>


#include "ADS1256.c"
#include "DAC8532.c" // 
#include "DEV_Config.c"


#include <functions.h> // RT Coding - Functions Put Here. 
#include <variables.h> // variables 

using namespace Eigen;

std::ofstream cur_meas("/home/baris/roboleg2/cur_meas.txt");


clock_t start, end;

float DT;

double motor_analog_old = 0;


static void do_rt_task(){
    
    //start = clock();

    auto start_time = std::chrono::high_resolution_clock::now(); // timer1 
    Rt = Rt + dt; // Realtime time. 


    // Analog Input 

   


    // Change here to adjust motor. 


        //chirp1 = Chirp(0,30,1.35,5,Rt-3)+1.65;

    cur_req = 1.5;


    if (Rt < 3) {
        cur = 0;
    }
    else if (Rt >= 3 & Rt < 3.25)
    {
        cur = cur_req*(Rt-3)/0.25;
    }
    else if (Rt >=3.25 & Rt <3.5)
    {
        cur = cur_req;}
    //else 
    //cur = cur_req;
    //}        
    else if (Rt >= 3.5 & Rt <3.6)
    {
        cur = cur_req;
        //cur = cur_req-cur_req*(Rt-3.5)/0.1;
    }
    else
    cur = cur_req;    
        
    

    // if cur is positive the spring compresses. 
        
        
    motor_analog_old = motor_analog_value;    
        

    
    // Current converted to Voltage 

    volt = dmdCur2ADCV(cur); //  -1.53 A to + 1.53 A ==> 0 V to 3.33 V

    // Analog Output 

    DAC8532_Out_Voltage(channel_A, volt); 
    

    DAC8532_Out_Voltage(channel_B, 0);
    
    if(isReady())   // The analog input gathered here from the analog output of the "ESCON"
        {
      
            motor_analog_value = ADS1256_GetChannalValue(1)*(5.0/0x7fffff);
            //motor_analog_value = (-1* motor_analog_value + 3.74 + 2 + 1.65);
            motor_analog_value = -1*motor_analog_value + 3.35;
        }

    // Voltage to Current of Analog Input
    
    
    
    if (motor_analog_value >=-0.5 & motor_analog_value <= 1.5)
    {   motor_analog_value = motor_analog_old; 
    }
    
     if (motor_analog_value >=3 & motor_analog_value <= 5)
    {   motor_analog_value = motor_analog_old; 
    }
    
    //motor_analog_value = motor_analog_value* (-1) + 2.90;
    
    
    cur_output = 0.612*(motor_analog_value) - 1.53;
    
     
    std::cout << Rt << "sec \t \t \t" << volt << "\t \t \t \t" << motor_analog_value << "\n"; // Terminal


     auto end_time = std::chrono::high_resolution_clock::now(); // timer1 
        
     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // timer1 

     cur_meas << Rt << " \t \t \t"<< float(duration.count())/1000  << "\t \t \t"  << cur  << " \t \t \t" << cur_output << "\t \t \t \t " << volt <<"\t \t \t \t" <<  motor_analog_value << "\t \t \t \t" << volt - (1.65* (-1*motor_analog_value + 2.65)/0.4) - 5.4 <<  std::endl; // printing to the txt file 
    
}

void  Handler(int signo)
{
    //System Exit
    printf("\r\nEND                  \r\n");
    DEV_ModuleExit();

    exit(0);
}











void* simple_cyclic_task(void* data)
{

    struct period_info pinfo;
    
    periodic_task_init(&pinfo);

    while (1) {
    
        //auto start_time = std::chrono::high_resolution_clock::now(); // timer1 
    
        do_rt_task();
        
        
        wait_rest_of_period(&pinfo);
        
        //auto end_time = std::chrono::high_resolution_clock::now(); // timer1 
        
        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // timer1 
        
        
        //std::cout << Rt << "\t \t \t" << float(duration.count())/1000 << std::endl;
        //cur_meas << Rt << "\t \t \t" << float(duration.count())/1000  <<  std::endl;      // timer1 
        //start = clock(); // timer 2

        //
        //DT = double(end - start)/double(CLOCKS_PER_SEC); // this show the loop time. Look for this, it needs to be locked at 1ms otherwise the control algorithm going to have a problem.  
        
        ////std::cout << DT << "\n"; // timer 2

 
        }
   
    
    return NULL;
    
    
    
}


int main(int argc, char* argv[])
{



    struct sched_param param;
    pthread_attr_t attr, ads1256_attr;
    pthread_t thread;
    int ret;


    DEV_ModuleInit();
    signal(SIGINT, Handler);


    
    ret = pthread_attr_init(&ads1256_attr);
    if (ret) {
    printf("init pthread attributes failed\n");
    goto out;
    }

    /* Set a specific stack size */
    ret = pthread_attr_setstacksize(&ads1256_attr, PTHREAD_STACK_MIN);
    if (ret) {
        printf("pthread setstacksize failed\n");
        goto out;
    }
    
    /* Set scheduler policy and priority of pthread */
    ret = pthread_attr_setschedpolicy(&ads1256_attr, SCHED_FIFO);
    if (ret) {
        printf("pthread setschedpolicy failed\n");
        goto out;
    }
    param.sched_priority = 80;
    ret = pthread_attr_setschedparam(&ads1256_attr, &param);
    if (ret) {
        printf("pthread setschedparam failed\n");
        goto out;
    }
    /* Use scheduling parameters of attr */
    ret = pthread_attr_setinheritsched(&ads1256_attr, PTHREAD_EXPLICIT_SCHED);
    if (ret) {
        printf("pthread setinheritsched failed\n");
        goto out;
    }

    /* Lock memory */
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
        printf("mlockall failed: %m\n");
        exit(-2);
    }

    /* Initialize pthread attributes (default values) */
    ret = pthread_attr_init(&attr);
    if (ret) {
        printf("init pthread attributes failed\n");
        goto out;
    }

    /* Set a specific stack size  */
    ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
    if (ret) {
        printf("pthread setstacksize failed\n");
        goto out;
    }

    /* Set scheduler policy and priority of pthread */
    ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if (ret) {
        printf("pthread setschedpolicy failed\n");
        goto out;
    }
    param.sched_priority = 80;
    ret = pthread_attr_setschedparam(&attr, &param);
    if (ret) {
        printf("pthread setschedparam failed\n");
        goto out;
    }
    /* Use scheduling parameters of attr */
    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (ret) {
        printf("pthread setinheritsched failed\n");
        goto out;
    }

    /* Create a pthread with specified attributes */
    ret = pthread_create(&thread, &attr, simple_cyclic_task, NULL);
    if (ret) {
        printf("create pthread failed\n");
        goto out;
    }

    /* Join the thread and wait until it is done */
    ret = pthread_join(thread, NULL);
    if (ret)
        printf("join pthread failed: %m\n");



out:

    return ret;
}


