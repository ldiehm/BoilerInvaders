#include "stm32f0xx.h"
#include "lcd.h"

void init_lcd_spi(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER11 | GPIO_MODER_MODER14 | GPIO_MODER_MODER3 | GPIO_MODER_MODER5);
	GPIOB->MODER |=  (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER3_1 | GPIO_MODER_MODER5_1);
	GPIOB->BSRR = (1<<8 | 1<<11 | 1<<14);

	GPIOB->AFR[0] &= ~0xf < (4 * 3);
	GPIOB->AFR[0] &= ~0xf < (4 * 5);

	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 &= ~SPI_CR1_SPE;
	SPI1->CR1 &= ~SPI_CR1_BR;
	SPI1->CR1 |= SPI_CR1_MSTR;
	SPI1->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM;
	SPI1->CR1 |= SPI_CR1_SPE;
	return;
}

void enable_ports(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8);

    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8);
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1 | GPIO_PUPDR_PUPDR8_1;

    return;

}


void rocketMan(void);

int main(void)
{
    enable_ports();
    LCD_Setup(); // this will call init_lcd_spi()

    generateGame();
    rocketMan();
}
