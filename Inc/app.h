/*
 * app.h
 *
 *  Created on: Jun 30, 2025
 *      Author: PRASHANT AJABE
 */

#ifndef APP_H_
#define APP_H_

#include "stm32f103x.h"

void app_rx0_handler(const can_rx_frame_t* msg);
void app_uart_rx_handler(uint8_t* rx_data);

#endif /* APP_H_ */
