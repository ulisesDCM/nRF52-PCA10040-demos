#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define MAX_FACTORIAL   10
/* 
    The output of the printk() is not deferred, meaning the output is 
    sent immediately to the console without any mutual exclusion or 
    buffering.
    Logs are sent immediately as they are issued, and printk() 
    will not return until all bytes of the message are sent
*/
void printFactorial(void)
{
    int factorial = 1;
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

int main(void)
{
    printk("Hello World!!!\n");

    while(1)
    {
        printk("Printing fatorial number to %d\n",MAX_FACTORIAL);
        printFactorial();
        k_msleep(1000);
    }
    return 0;
}