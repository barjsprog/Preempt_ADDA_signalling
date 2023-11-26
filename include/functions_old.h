


#define curK 1.07843 
#define curB 1.65

#define curK2 0.72368
#define curB2 1.65

std::ofstream time_calc("/home/baris/roboleg2/time_calc.txt");

double dmdCur2ADCV(double dmdcur) {   // dmdcur :demand current for ball screw motor
    
    return curK*dmdcur + curB;
}

double dmdCur2ADCV2(double dmdcur2) {   // dmdcur 2:demand current for double sided motor 
    
    return curK2*dmdcur2 + curB2;
}



double Chirp(double w1, double w2, double A,double M,double time)
{
	double res;
	res = A*sin(w1*time + (w2-w1)*time*time/(2*M));
	return res;
}

double ChirpCos(double w1, double w2, double A,double M,double time)
{
	double res;
	res = A*cos(w1*time + (w2-w1)*time*time/(2*M));
	return res;
}


////UDOUBLE ADC[8],i;
//void usleep(unsigned int microseconds) {
    //struct timespec delay;
    //delay.tv_sec = microseconds / 1000000;
    //delay.tv_nsec = (microseconds % 1000000) * 1000;
    
    //nanosleep(&delay, NULL);
//}


struct period_info {
    struct timespec next_period;
    long period_ns;
};



static void inc_period(struct period_info* pinfo)
{
    pinfo->next_period.tv_nsec += pinfo->period_ns;

    while (pinfo->next_period.tv_nsec >= 1000000000) {
        /* timespec nsec overflow */
        pinfo->next_period.tv_sec++;
        pinfo->next_period.tv_nsec -= 1000000000;

    }
}


// Adjust sampling size from here .
static void periodic_task_init(struct period_info* pinfo)
{

    /* or simplicity, hardcoding a 1ms period */
    pinfo->period_ns = 1000000;
    //pinfo->period_ns = 100000000;
    clock_gettime(CLOCK_REALTIME, &(pinfo->next_period)); // CLOCK_MONOTONIC, CLOCK_REALTIME

}




//static void wait_rest_of_period(struct period_info* pinfo)
//{
    //struct timespec current_time;
    //struct timespec remaining_time;
    
    
    
    //inc_period(pinfo);
    

        //remaining_time.tv_sec = pinfo->next_period.tv_sec - current_time.tv_sec;
        //remaining_time.tv_nsec =pinfo->next_period.tv_nsec - current_time.tv_nsec;

        
            

    ///* for simplicity, ignoring possibilities of signal wakes */
    //clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &remaining_time, NULL); // CLOCK_MONOTONIC, CLOCK_REALTIME

//}



static void wait_rest_of_period(struct period_info* pinfo)
{
    

    inc_period(pinfo);

    /* for simplicity, ignoring possibilities of signal wakes */
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &pinfo->next_period, NULL); // CLOCK_MONOTONIC, CLOCK_REALTIME
    

    
    
}




