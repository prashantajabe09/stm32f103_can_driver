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
	SCB->VTOR = 0x08004000;
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


void print_msg(char* format,...)
{
	char str[80];
	va_list args;
	va_start(args,format);
	vsprintf(str,format,args);
	usart_2_handle.tx_buffer = &(str);
	usart_write_polling(&usart_2_handle);
}
