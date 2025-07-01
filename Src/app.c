/*
 * app.c
 *
 *  Created on: Jun 30, 2025
 *      Author: PRASHANT AJABE
 */

#include "app.h"

void app_rx0_handler(const can_rx_frame_t* msg)
{
	sprintf(tx_buffer_data,"ID: 0x%x \r\n",msg->id);
	usart_2_handle.tx_buffer = &(tx_buffer_data);
	usart_write_polling(&usart_2_handle);

	sprintf(tx_buffer_data,"INDEX: 0x%x \r\n",msg->filter_match_index);
	usart_2_handle.tx_buffer = &(tx_buffer_data);
	usart_write_polling(&usart_2_handle);

	sprintf(tx_buffer_data,"Data: ");
	usart_2_handle.tx_buffer = &(tx_buffer_data);
	usart_write_polling(&usart_2_handle);
	usart_2_handle.tx_buffer = &(msg->data_8[0]);
	usart_write_polling(&usart_2_handle);

	sprintf(tx_buffer_data,"\r\n");
	usart_2_handle.tx_buffer = &(tx_buffer_data);
	usart_write_polling(&usart_2_handle);

}


