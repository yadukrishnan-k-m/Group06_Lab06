void Sys_Port_Initialisation(void){

    // PORTF, PF7-PF0, PF4-SW1, PF3-green, PF2-blue, PF1-red, PF0-SW2
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // Enable clock for Port F
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;      // Unlock Port F
    GPIO_PORTF_CR_R = 0x1f;                 // Commit changes,1-enable (PF7-PF0 = 00011111)
    GPIO_PORTF_DEN_R = 0x1f;                // Digital function enable, 1-enable (PF7-PF0 = 00011111)
    GPIO_PORTF_DIR_R = 0x0e;                // Set output/input, 1-output (PF7-PF0 = 00001110)
    GPIO_PORTF_PUR_R = 0x11;                // Enable pull-up resistor, 1-enable (PF7-PF0 = 00010001)
    GPIO_PORTF_DATA_R = 0x00;               // Reset the data register (PF7-PF0 = 00000000)

}

void PWMConfiguration(void)
{
    SYSCTL_RCGCPWM_R |= (1<<1);             // Enable PWM1 clock
    GPIO_PORTF_AFSEL_R |= (1<<2);           // Enable GPIO alternate function
    GPIO_PORTF_PCTL_R |= 0x500;             // Make PF2 as PWM output

    // Configure the PWM generator
    PWM1_3_CTL_R |= 0x00;                   // Disable PWM3 while configuring and select down count mode
    PWM1_3_GENA_R = 0x8C;                   // Set PWM3A to produce a symmetric down-counting PWM signal
    PWM1_3_LOAD_R = time_period;
    PWM1_3_CMPA_R = (duty/100)*time_period - 1;
    PWM1_3_CTL_R |= 0x01;                   // Enable generator 3 counter
    PWM1_ENABLE_R |= 0x040;                 // Enable PWM1 channel 6 Output
}