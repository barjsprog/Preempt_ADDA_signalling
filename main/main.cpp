#include "roboleg_func.cpp"



int main(int argc, char* argv[])
{

    struct sched_param param;
    pthread_attr_t attr, ads1256_attr;
    pthread_t thread;
    int ret;


    DEV_ModuleInit();
    signal(SIGINT, Handler);
    
    
    
    
    uint8_t id;
    uint8_t samplerate = 4;
    uint8_t readchannel = 1;
    
    ADS1256_Reset();                        // reset the chip
    id = ADS1256_ReadChipID();
    
    if (id != 3)   {
        printf("Error, ASD1256 Chip ID = 0x%d\n", (int)id);
        return 1;
    }
   
    ADS1256_CfgADC(ADS1256_GAIN_1, (ADS1256_DRATE_E) samplerate);
    ADS1256_CfgADC(ADS1256_GAIN_1, (ADS1256_DRATE_E) samplerate);
    
 
    ADS1256_SetChannel(readchannel);	// select ADC channel (0..7)
    bsp_DelayUS(5);
    ADS1256_WriteCmd(CMD_SYNC);  // SYNC needed after changing input MUX
    bsp_DelayUS(5);
    ADS1256_WriteCmd(CMD_WAKEUP); // restart ADC after SYNC operation
    bsp_DelayUS(25);

    ADS1256_WriteCmd(CMD_RDATAC); // start continuous reading mode
    bsp_DelayUS(5);
    
    
    
 


    
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


