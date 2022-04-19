/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"

void init_ports(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOC->MODER &= ~GPIO_MODER_MODER6;
    GPIOC->MODER |= GPIO_MODER_MODER6_0;

    GPIOA->MODER &= ~GPIO_MODER_MODER6;
    GPIOC->MODER &= ~GPIO_MODER_MODER7;
    GPIOC->MODER |= GPIO_MODER_MODER7_0;

    GPIOA->MODER &= ~GPIO_MODER_MODER7;
    GPIOC->MODER &= ~GPIO_MODER_MODER8;
    GPIOC->MODER |= GPIO_MODER_MODER8_0;

    //GPIOA->MODER |= GPIO_MODER_MODER5_0;
    //GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;
    //GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5;
    //GPIOA->MODER |= GPIO_MODER_MODER5_1;
}

void buttontest(){
    int check5;
    check5 = GPIOA->IDR & 0x20;
    if(check5 == 0x20){
        GPIOC->BSRR = 0x40;
    }
    else{
        GPIOC->BRR = 0x40;
    }

    int check6;
    check6 = GPIOA->IDR & 0x40;
    if(check6 == 0x40){
        GPIOC->BSRR = 0x80;
    }
    else{
        GPIOC->BRR = 0x80;
    }

    int check7;
    check7 = GPIOA->IDR & 0x80;
    if(check7 == 0x80){
        GPIOC->BSRR = 0x100;
    }
    else{
        GPIOC->BRR = 0x100;
    }

}

int main(void)
{

	init_ports();
	for(;;){
	    buttontest();
	}

}
