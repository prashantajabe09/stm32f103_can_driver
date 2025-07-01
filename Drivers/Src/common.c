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

	uart_x_configure_parameter(&usart_2_handle);
	usart_init(&usart_2_handle);

	hal_can_init(&can_configuration);

	systick_init();

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
