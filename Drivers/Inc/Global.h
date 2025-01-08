/*
 * Global.h
 *
 *  Created on: 29-Nov-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

/*
 * Global Macros
 */
#define NOT_APPLICABLE 0
#define ENABLE 		   1
#define DISABLE        0
#define SET 		   1
#define RESET          0

/*
 * Global Variable
 */
extern uint16_t capture_value;
extern uint8_t flag_send_data;
extern uint8_t tx_buffer_data[];
extern uint8_t rx_buffer_data[];
extern uint16_t tx_buffer_data_len;
extern uint16_t rx_buffer_data_len;
extern uint8_t tc_buffer[],tc_buffer_index;
extern uint8_t txe_buffer[],txe_buffer_index;

#endif /* INC_GLOBAL_H_ */
