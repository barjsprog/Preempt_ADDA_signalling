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


#include "ADS1256_read.c"
//#include "ADS1256.c"
#include "DAC8532.c" 
#include "DEV_Config.c"


#include <functions.h> // RT Coding - Functions Put Here. 
 // variables 

using namespace std;
using namespace std::chrono;
using namespace Eigen;


struct timespec t_start, t_end;
long desired_period_ns = 1000000;

struct period_info { 
	struct timespec next_period;
	long period_ns;
};


static void do_rt_task(double period);
void  Handler(int signo);
static void inc_period(struct period_info* pinfo);
static void periodic_task_init(struct period_info* pinfo);
static void wait_rest_of_period(struct period_info* pinfo);
void* simple_cyclic_task(void* data);


//float DT;


double motor_analog_old = 0.25;
double prev_motor_analog = 0.25 ;

// Variables

double cur_output = 0 ;

 // define current here, -1.53 to 1.53

double chirp1 = 1.65;
double cur = 0;

double dt = 0.001;

double volt = 0; 
double volt2 = 0; 
double motor_analog_value ;


float x;

double cur_req = 0;


double P = 100;

double err = 0; 
