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


#include <ADS1256.c>
#include "DAC8532.c" // 
#include "DEV_Config.c"


#include <functions.h> // RT Coding - Functions Put Here. 
#include <variables.h> // variables 

using namespace Eigen;

std::ofstream cur_meas("/home/baris/roboleg2/cur_meas.txt");


clock_t start, end;



    
static void do_rt_task(){
    
   



    DAC8532_Out_Voltage(channel_A, 1.65); 

    DAC8532_Out_Voltage(channel_B, 0);
    
    
    
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
    
        auto start_time = std::chrono::high_resolution_clock::now(); // timer1 
    
        do_rt_task();
        
        
        wait_rest_of_period(&pinfo);
        
        auto end_time = std::chrono::high_resolution_clock::now(); // timer1 
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // timer1 
        
        //cur_meas << float(duration.count())/1000  <<  std::endl;      // timer1 
        //start = clock(); // timer 2

        //end = clock(); // timer 2
        //DT = double(end - start)/double(CLOCKS_PER_SEC); // this show the loop time. Look for this, it needs to be locked at 1ms otherwise the control algorithm going to have a problem.  
        
        ////std::cout << DT << "\n"; // timer 2
        //cur_meas << DT  << "\t \t"<< DT2<< std::endl; // timer 2
 
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



//#include "/home/baris//DEV_Config.h"
//#include "/home/baris/roboleg/DAC8532.h"
//#include "/home/baris/Test/bcm/Debug.h"





//static void do_rt_task()
//{
//
//    if (isReady())
//    {
//
//        //ADS1256_GetAll(ADC);
//
//        ////UDOUBLE ADC[8],i;
//        ////float x;
//        //x = (ADC[1] >> 7)*5.0/0xffff;
//        ////printf(" %f \r \n ", x);
//        ////printf("\33[9A");//Move the cursor up 8 lines
//
//         //printf("0 : %f\r\n",ADS1256_GetChannalValue(0)*5.0/0x7fffff);
//         //printf("1 : %f\r\n",ADS1256_GetChannalValue(1)*5.0/0x7fffff);
//         //printf("2 : %f\r\n",ADS1256_GetChannalValue(2)*5.0/0x7fffff);
//         //printf("3 : %f\r\n",ADS1256_GetChannalValue(3)*5.0/0x7fffff);
//         //printf("4 : %f\r\n",ADS1256_GetChannalValue(4)*5.0/0x7fffff);
//         //printf("5 : %f\r\n",ADS1256_GetChannalValue(5)*5.0/0x7fffff);
//         //printf("6 : %f\r\n",ADS1256_GetChannalValue(6)*5.0/0x7fffff);
//         //printf("7 : %f\r\n",ADS1256_GetChannalValue(7)*5.0/0x7fffff);
//
//        analog = ADS1256_GetChannalValue(1) * (10.0 / 0x7fffff);
//        analog = (-1 * analog + 3.74 + 2 + 1.65);
//
//    }
//    else
//
//
//
//        //for(i=0;i<8;i++)
//        //{
//            //printf("%d %f\r",i,ADC[i]*5.0/0x7fffff);
//        //}
//
//
//        volt = 1.078 * cur + 1.65;
//
//
//    if (Rt < 3) {
//        cur = 0;
//    }
//    else
//        cur = 1.53;
//
//    //chirp1 = Chirp(0,30,1.35,5,Rt-3)+1.65;
//
//
//    Rt = Rt + dt;
//    volt = 1.078 * cur + 1.65;
//
//
//
//
//    //printf(" %f %f %f \n " , Rt,chirp1,-1*analog+1.35+1.65+1.55);
//
//    //printf("Value: %f \n",analog);
//
//
//    DAC8532_Out_Voltage(channel_B, 0);
//
//    DAC8532_Out_Voltage(channel_A, volt); // chirp1
//
//    cur_output = 0.765 * (analog)-1.53;
//
//
//    //usleep(1000);
//    ////printf("demo\r\n");
//
//    ////printf("Program start\r\n");
//
//        ////if(ADS1256_init() == 1){
//        ////printf("\r\nEND                  \r\n");
//        ////DEV_ModuleExit();
//        ////exit(0);
//    ////}
//
//
//    std::cout << Rt << "sec \t \t \t" << volt << "\n"; // printing to the terminal
//
//
//
//    //printf("%d %f\r\n",i,ADC[i]*5.0/0x7fffff);
//    //std::cout << ADC[1] << " \n";
//
//
//    //x = (ADC[0] >> 7)*5.0/0xffff;
//    //printf(" %f \r\n", x);
//    //printf("\33[9A");//Move the cursor up 8 lines
//
//
//    //cur_meas << Rt << " \t \t \t" << cur << "\t \t \t \t " << cur_output << std::endl;
//    cur_meas << Rt << " \t \t \t" << volt << "\t \t \t \t " << analog << std::endl; // printing to the txt file 
//}
//
