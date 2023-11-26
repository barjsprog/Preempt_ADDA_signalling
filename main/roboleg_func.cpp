#include "roboleg_func.hpp"
#include <unistd.h>


std::ofstream cur_meas("/home/baris/roboleg2/cur_meas.txt");

clock_t start, end;


double LPF(double Input, double prevOut, double freq, double dt)
{
    double out = (dt*freq* Input + prevOut)/(1+freq*dt);
    return out;
    
    
    }

double TF(double x,double d,double dt,double a,double b, double c,double yp,double ypp)
{
    double out = (d*x*dt*dt*2 + b*yp*dt + 2*a*yp - a*ypp)/(c*dt*dt + b*dt + a);
    
    
}
#define COUNTSPERVOLT 1695929
double lpf_motor_analog_value  = 0.25;
double prev_lpf_motor_analog_value  = 0.25 ;
double prev_prev_lpf_motor_analog_value = 0.25;
double  tf_motor_analog_value = 0;
double prev_prev_tf_motor_analog_value = 0;
double prev_tf_motor_analog_value = 0;
double lpff_prev = 0;
double lpff = 0;
 double   prevprev_volt_give = 0;
 double prev_volt_give = 0;
double volt_give = 0;
double cur_PID = 0;
double Kp = 200;
double Kd = 0;
double prev_err = 0;

double Rt = 0;

int printout  =1 ;

static void do_rt_task(double period ){
    
    //start = clock();

    auto start_time = std::chrono::high_resolution_clock::now(); // timer1 
    Rt = Rt + dt; // Realtime time. 


    // Analog Input 

   


    // Change here to adjust motor. 


        //chirp1 = Chirp(0,30,1.35,5,Rt-3)+1.65;

    cur_req = 1.0;


    if (Rt < 1.75) {
        cur = 0;
    }
    else if (Rt >= 1.75 & Rt < 2)
    {
        cur = cur_req*(Rt-1.75)/0.25;
    }
    else
    {
        cur = cur_req;

    }

    // cur = -1.53;
    
    
    
    
    
    // if cur is positive the spring compresses. 
        
    //cur = cur_req;
    motor_analog_old = motor_analog_value;    
        

    
    // Current converted to Voltage 

    volt = dmdCur2ADCV(cur);          // ANKLE BSM MOTOR  //  -1.53 A to + 1.53 A ==> 0 V to 3.33 V

    // volt = 0.453*(cur) + 1.65; // KNEE BSM MOTOR

    
    //volt = 1.65; // bsm 
    volt2 = 1.65; // replacer
    
    // Analog Output 

    DAC8532_Out_Voltage(channel_A, volt); 
    

    DAC8532_Out_Voltage(channel_B,volt2);
    
    
    
    ADS1256_WaitDRDY();  // wait for ADC result to be ready
    int32_t raw = ADS1256_ReadDataCont(); // get new ADC result
    
    
    motor_analog_value =(long) raw;
    motor_analog_value = motor_analog_value / COUNTSPERVOLT;

    
    
    //if (printout != 0) printf("%8ld\n", raw);
    
    
    //if(isReady())   // The analog input gathered here from the analog output of the "ESCON"
        //{
               //motor_analog_value = ADS1256_GetChannalValue(1)*(5.0/0x7fffff);
    
        //}
    
    //while(n < readcount)  // ===== MAIN READING LOOP ===========
    //{

        //  // raw ADC value as integer

        //if (raw > sMax) sMax = raw;  // remember max & min values seen so far
        //if (raw < sMin) sMin = raw;

        //x = (double) raw;          // convert to floating point
        //datSum += x;
              //// from http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
        //n++;
        //delta = x - mean;
        //mean += delta/n;
        //m2 += (delta * (x - mean));
    //}  // ============= END MAIN READING LOOP ==============
    
    
    
    //motor_analog_value = (-1* motor_analog_value + 3.74 + 2 + 1.65);
    //motor_analog_value = motor_analog_value ;
    
    //prev_prev_lpf_motor_analog_value = prev_lpf_motor_analog_value;
   
    //prev_lpf_motor_analog_value = lpf_motor_analog_value;
    

    
    //lpf_motor_analog_value = LPF(motor_analog_value, prev_lpf_motor_analog_value, 30*3.14 ,period);
    
    

    
    
    

    
    // tf_motor_analog_value = TF(cur_PID,0.1829,period,1,0.5079,0.1751,prev_tf_motor_analog_value, prev_prev_tf_motor_analog_value);
    //tf_motor_analog_value = TF(cur_PID,14840,period,1,213.8,13.780,prev_tf_motor_analog_value, prev_prev_tf_motor_analog_value);
    
    
    //prev_err = err;
    
    //err = cur - tf_motor_analog_value;
    
    //cur_PID = Kp * (err) + Kd * ((err - prev_err)/period) ;
    


    
    //if(err >= 2)
    //{motor_analog_value = prev_motor_analog;}
    
        
    
    //if(motor_analog_value >= 4)
    //{motor_analog_value = prev_motor_analog;}
    
    
    //if(motor_analog_value < 3.5)
    //{motor_analog_value = prev_motor_analog;}


    // Voltage to Current of Analog Input
    
    
    
    //if (motor_analog_value >=-0.5 & motor_analog_value <= 1.5)
    //{   motor_analog_value = motor_analog_old; 
    //}
    
     //if (motor_analog_value >=3 & motor_analog_value <= 5)
    //{   motor_analog_value = motor_analog_old; 
    //}
    
    //motor_analog_value = motor_analog_value* (-1) + 2.90;
    
    
    cur_output = (motor_analog_value) ;
    
     


     //auto end_time = std::chrono::high_resolution_clock::now(); // timer1 
        
     //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // timer1 

    std::cout << Rt << "\s sec \t\t input_1:\t" << cur << "\t measurement:\t" <<  cur_output <<"\t\t\t \n"; // Terminal

    //cur_meas << Rt << " \t \t \t"  << cur << " \t \t \t \t"  << cur_output   <<"\t \t \t \t" << volt << "\t \t \t" <<  motor_analog_value << "\t \t \t \t"  <<  std::endl; // printing to the txt file 
    
    
    //std::cout << "killed \n" ;
    //exit(0);
    // float(duration.count())/1000 
    // float(duration.count())/1000  << "\t \t \t"
     
}

void  Handler(int signo)
{
    //System Exit
    printf("\r\nEND                  \r\n");
    DEV_ModuleExit();

    exit(0);
}






static void inc_period(struct period_info* pinfo)
{
    pinfo->next_period.tv_nsec += pinfo->period_ns;

    while (pinfo->next_period.tv_nsec >= 1000000000) {
        /* timespec nsec overflow */
        pinfo->next_period.tv_sec++;
        pinfo->next_period.tv_nsec -= 1000000000;

    }
}


static void periodic_task_init(struct period_info* pinfo)
{

    /* or simplicity, hardcoding a 1ms period */
    pinfo->period_ns = desired_period_ns;
    //pinfo->period_ns = 100000000;
    clock_gettime(CLOCK_REALTIME, &(pinfo->next_period)); // CLOCK_MONOTONIC, CLOCK_REALTIME

}

static void wait_rest_of_period(struct period_info* pinfo)
{
    

    inc_period(pinfo);

    /* for simplicity, ignoring possibilities of signal wakes */
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &pinfo->next_period, NULL); // CLOCK_MONOTONIC, CLOCK_REALTIME
    

    
    
}


void* simple_cyclic_task(void* data)
{

    struct period_info pinfo;
    long duration = desired_period_ns;
    double rt_period_sec = 0.01;
    
    periodic_task_init(&pinfo);

    while (1) {
    
        auto start_time = std::chrono::high_resolution_clock::now(); // timer1 
        rt_period_sec = (double) (duration*1e-9);
        clock_gettime(CLOCK_MONOTONIC , &t_start);
        
        do_rt_task(rt_period_sec);
        wait_rest_of_period(&pinfo);
        
        clock_gettime(CLOCK_MONOTONIC , &t_end);
        duration = (t_end.tv_nsec - t_start.tv_nsec);
        
        auto end_time = std::chrono::high_resolution_clock::now(); // timer1 
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time); // timer1 
        rt_period_sec = (double)(duration.count());
        
        //std::cout << Rt << "\t \t \t" << float(duration.count())/1000 << std::endl;
        cur_meas << Rt << "\t \t \t" << float(duration.count())/1000  <<  std::endl;      // timer1 
        //start = clock(); // timer 2

        //
        //DT = double(end - start)/double(CLOCKS_PER_SEC); // this show the loop time. Look for this, it needs to be locked at 1ms otherwise the control algorithm going to have a problem.  
        
        ////std::cout << DT << "\n"; // timer 2

 
        }
   
    
    return NULL;
    
    
   }
