#include <stdio.h>
#include <zephyr/kernel.h>

#include <zephyr/sys/printk.h>

#include <zephyr/logging/log.h>

#define MAX_FACTORIAL   10
/* 
    if you set the log level of module X to  LOG_LEVEL_DBG, it means all
    messages generated (Debug, Info, Warning, and Error) will be send to 
    the console. On the other hand, if you set the maximal log level of
    module Y to LOG_LEVEL_WRN, it means only messages with a severity 
    level of warning and error will be sent to the console. 
    If the minimum log level is not provided, then default global 
    log level (CONFIG_LOG_DEFAULT_LEVEL) is used in the file. 
    The default global log level is set to LOG_LEVEL_INF.
    
 */
LOG_MODULE_REGISTER(factorial_exer, LOG_LEVEL_DBG);

/* 
    The output of the printk() is not deferred, meaning the output is 
    sent immediately to the console without any mutual exclusion or 
    buffering.
    Logs are sent immediately as they are issued, and printk() 
    will not return until all bytes of the message are sent
*/
void printkFactorial(void)
{
    int factorial = 1;
    printk("Printing fatorial number to %d\n",MAX_FACTORIAL);
    for(uint8_t i=1; i<=MAX_FACTORIAL;  i++)
    {
        factorial =1;
        for(uint8_t j=1; j<=i; j++)
        {
            factorial = factorial*j;
        }
        printk("The factorial of %2d = %d \n",i,factorial);
    }
}

void logFactorial(void)
{
    int exercise_num=2;
    uint8_t data[] = {0x00, 0x01, 0x02, 0x03,
                      0x04, 0x05, 0x06, 0x07,
                      'H', 'e', 'l', 'l','o'};
    //Printf-like messages
    LOG_INF("Exercise %d",exercise_num);    
    LOG_DBG("A log message in debug level");
    LOG_WRN("A log message in warning level!");
    LOG_ERR("A log message in Error level!");
    //Hexdump some data
    LOG_HEXDUMP_INF(data, sizeof(data),"Sample Data!");     

    int factorial = 1;
    LOG_INF("Printing fatorial number to %d",MAX_FACTORIAL);
    for(uint8_t i=1; i<=MAX_FACTORIAL;  i++)
    {
        factorial =1;
        for(uint8_t j=1; j<=i; j++)
        {
            factorial = factorial*j;
        }
        LOG_INF("The factorial of %2d = %d",i,factorial);
    }

}

int main(void)
{
    while(1)
    {
        // printkFactorial();
        // logFactorial();
        LOG_INF("Hello world!!!");
        k_msleep(1000);
    }
    return 0;
}