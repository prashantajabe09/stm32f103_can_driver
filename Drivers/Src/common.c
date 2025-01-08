/*
 * common.c
 *
 *  Created on: 30-Jul-2024
 *      Author: Prashant Ajabe
 */
#include "common.h"

void mcu_init(void)
{
	clock_init();
	io_init();
	i2cx_init(I2C2,I2C2_CHANNEL);
	uart_x_configure_parameter(&usart_2_handle);
	usart_init(&usart_2_handle);
	spi_init(SPI1,SPI_1);
	timer_channel_init(TIMER1,TIMER_CHANNEL_1);
	timer_channel_init(TIMER1,TIMER_CHANNEL_2);
	dmax_channel_init(DMA1,CHANNEL_4);
	dmax_channel_init(DMA1,CHANNEL_5);
	dmax_channel_init(DMA1,CHANNEL_7);
	dmax_channel_init(DMA1,CHANNEL_6);

}

uint32_t round_off(float number)
{
	float fraction;
	uint8_t carry;
	uint32_t realPart = (int)number;
	fraction = number - (int)number;
	if (fraction < 0.50)
	{
		carry = 0;
	}
	else if (fraction >= 0.50)
	{
		carry = 1;
	}

	return (realPart + (uint32_t)carry);
}
