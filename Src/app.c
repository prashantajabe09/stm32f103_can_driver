/*
 * app.c
 *
 *  Created on: Jun 30, 2025
 *      Author: PRASHANT AJABE
 */

#include "app.h"

void app_rx0_handler(const can_rx_frame_t* msg)
{
	print_msg("ID:0x%x \r\n",msg->id);

	print_msg("INDEX:0x%x \r\n",msg->filter_match_index);

	print_msg("Data: ");
	for(uint8_t i = 0; i < msg->length;i++)
	{
		print_msg("%c",msg->data_8[i]);
	}
	print_msg("\r\n");
}


