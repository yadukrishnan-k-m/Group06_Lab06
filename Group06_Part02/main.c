// It includes both the functionalities in the question (b). The SW1 will function as short press and long press
// A 100kHz PWM varying duty cycle waveform on Pin PF2(blue led); controlled by Module 1 PWM Generator 3 A
// SW1 is used to reduce the brightness by the step of 5 % in long press. Increase 5% in short press



#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

#define SWITCH1    (1U << 4) // PF4

int i = 0;
int j = 0;

//Interrupt Handlers
void GPIOFHandler(void);
void SysTickHandler(void){}

void Sys_Port_Initialisation(void);
void GPIOinterrupt_Initialisation(void);
void PWMConfiguration(void);
void delay_ms(int milliseconds);
void button_read (void);

#define frequency 100000                   // 100 kHz frequency
#define time_period (16000000/frequency)   // 16MHz clock frequency

volatile int duty = 50;                    // Duty cycle in percentage

void GPIOFHandler(void)
{
    if(GPIO_PORTF_MIS_R == 0x10){           // Interrupt from SW1
        button_read ();
        //GPIO_PORTF_DATA_R ^= 0x08;          // Toggle data in PF1 (green led)
    }

    /*if(GPIO_PORTF_MIS_R == 0x01){           // Interrupt from SW2
        //GPIO_PORTF_DATA_R ^= 0x02;          // Toggle data in PF1 (red led)
        if(duty != 0) duty = duty-5;        // Decreasing duty cycle by 5 %
        if(duty < 0) duty = 0;
        PWM1_3_CMPA_R = (time_period * duty) / 100;
    }*/

    GPIO_PORTF_ICR_R = 0x11;                // Interrupt clear, 1-clear all prior interrupts (PF7-PF0 = 00010001)
}

void button_read (){
    if((GPIO_PORTF_DATA_R & SWITCH1) == 0){
        delay_ms(1);
        if((GPIO_PORTF_DATA_R & SWITCH1) == 0){
            delay_ms(20);
            if((GPIO_PORTF_DATA_R & SWITCH1) != 0){
                if(duty != 0) duty = duty-5;     // Decreasing duty cycle by 5 %
                if(duty < 0) duty = 0;
                PWM1_3_CMPA_R = (time_period * duty) / 100;
                //GPIO_PORTF_DATA_R ^= 0x02;     // Toggle data in PF1 (red led)
                delay_ms(5);
            }
            else{
                delay_ms(100);
                if((GPIO_PORTF_DATA_R & SWITCH1) == 0){
                    if(duty != 100)duty = duty+5;       // Increasing duty cycle by 5 %
                    if(duty > 100) duty = 100;
                    if(duty == 100) duty = 99;
                    PWM1_3_CMPA_R = (time_period * duty) / 100;
                    //GPIO_PORTF_DATA_R ^= 0x08;        // Toggle data in PF4 (green led)
                    delay_ms(5);
                }
            }
        }
    }
}