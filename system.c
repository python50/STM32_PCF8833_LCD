#include <stdint.h>
#include "stm32f10x.h"

#include "gpio_macro.h"

void init_clock()
{
	FLASH->ACR|=FLASH_ACR_LATENCY_2;

	//PLL 16x internal clock /2. 64Mhz
	//APB1, low speed peripheral bus divided by 2. 32Mhz
	//ADC Clock Divided by 6. 10.66Mhz
	//RCC_CFGR_PLLMULL | 
	//RCC->CFGR=RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_ADCPRE_DIV8;
	RCC->CFGR|=RCC_CFGR_PLLMULL|RCC_CFGR_PPRE1_DIV2|RCC_CFGR_ADCPRE_DIV6;
	
	//Enable PLL
	RCC->CR|=RCC_CR_PLLON;


	//RCC->APB1ENR=	;
	RCC->APB2ENR=	RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
	
	//Wait for PLL lock
	while(! (RCC->CR & RCC_CR_PLLRDY)) {}
	
	//PLL Selected as main clock;
	RCC->CFGR|=RCC_CFGR_SW_PLL;	
}


void init_gpio()
{
	//PA0	RESET (Active Low)
	//PA1	CE (Active Low)
	//PA2	USART2 TX
	//PA3	USART2 RX Not enabled
	//PA4	USART2 CLK
	//PA5	LED
	//PA8	USART1 CLK
	//PA9	USART1 TX
	//PA10	USART1 RX
	GPIOA->CRL=	GPIO_CRL(0,	GPIO_MODE_OUTPUT,	GPIO_OCFG_IO)|
			GPIO_CRL(1,	GPIO_MODE_OUTPUT,	GPIO_OCFG_IO)|
			GPIO_CRL(2,	GPIO_MODE_OUTPUT,	GPIO_OCFG_ALT)|
			GPIO_CRL(3,	GPIO_MODE_INPUT,	GPIO_ICFG_FLOAT)|
			GPIO_CRL(4,	GPIO_MODE_OUTPUT,	GPIO_OCFG_ALT)|
			GPIO_CRL(5,	GPIO_MODE_OUTPUT,	GPIO_OCFG_IO);
			
	GPIOA->CRH=	GPIO_CRH(8,	GPIO_MODE_OUTPUT,	GPIO_OCFG_ALT)|
			GPIO_CRH(9,	GPIO_MODE_OUTPUT,	GPIO_OCFG_ALT)|
			GPIO_CRH(10,	GPIO_MODE_INPUT,	GPIO_ICFG_FLOAT);
}

void init_usart1()
{
	USART1->BRR=1<<5;
	
	//Syncronous Comms Enable
	//enable clock pulse on last bit
	USART1->CR2=USART_CR2_CLKEN | USART_CR2_LBCL;

	//Usart enable
	//transmit enable

	//9 bit mode
	USART1->CR1=USART_CR1_TE|USART_CR1_UE|USART_CR1_M;
}

void usart1_send(uint16_t data)
{
	while(!(USART1->SR&USART_SR_TXE)) {}
	
	USART1->DR=data;
}
