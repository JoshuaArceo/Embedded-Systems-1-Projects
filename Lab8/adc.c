/*
 * adc.c
 *
 *  Created on: Mar 20, 2024
 *      Author: jarceo
 */

#include "Timer.h"

void adc_init()
{
    static uint8_t initialized = 0;

    if (initialized)
    {
        return;
    }

    SYSCTL_RCGCADC_R |= 0X0001;
    SYSCTL_RCGCGPIO_R |= 0X02;

    while ((SYSCTL_RCGCGPIO_R & 0X02) == 0)
    {
    }

    GPIO_PORTB_DIR_R &= ~0X10;
    GPIO_PORTB_AFSEL_R |= 0X10;

    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    while ((SYSCTL_PRADC_R & 0x0001) != 0x0001)
    {
    };

    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1; // 8) configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123; // 9) Sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008; // 10) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000; // 11) seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10; // 12) set channel
    ADC0_SSCTL3_R = 0x0006; // 13) no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008; // 14) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008; // 15) enable sample sequencer 3
    ADC0_SAC_R = 4;

    initialized = 1;
}

uint32_t adc_read(void)
{
    uint32_t result;
    ADC0_PSSI_R = ADC_PSSI_SS3; // 1) initiate
    while ((ADC0_RIS_R & ADC_RIS_INR3) == 0)
    {
    };
    result = ADC0_SSFIFO3_R & ADC_SSFIFO0_DATA_M; // 3) read
    ADC0_ISC_R = ADC_ISC_IN3;
    return result;
}



