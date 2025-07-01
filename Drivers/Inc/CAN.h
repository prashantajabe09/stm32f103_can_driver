/*
 * CAN.h
 *
 *  Created on: Jun 5, 2025
 *      Author: PRASHANT AJABE
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "stm32f103x.h"

#define STANDARD_FRAME	0
#define EXTENDED_FRAME	1

#define RX_DONE			1
#define TX_DONE			1

#define SILENT_TEST_MODE_BIT_POSITION		31
#define LOOPBACK_TEST_MODE_BIT_POSITION		30

#define SILENT_TEST_MODE_DI					0
#define SILENT_TEST_MODE_EN					1
#define LOOPBACK_TEST_MODE_DI				2
#define LOOPBACK_TEST_MODE_EN				3

#define SILENT_AND_LOOPBACK_TEST_MODE		(3 << LOOPBACK_TEST_MODE_BIT_POSITION)

#define TRANSMIT_MAILBOX_0_EMPTY_BIT_POS	26
#define TRANSMIT_MAILBOX_1_EMPTY_BIT_POS	27
#define TRANSMIT_MAILBOX_2_EMPTY_BIT_POS	28

#define TRANSMIT_MAILBOX_0					 0
#define TRANSMIT_MAILBOX_1					 1
#define TRANSMIT_MAILBOX_2					 2
#define TRANSMIT_MAILBOX_NOT_EMPTY			 -1

#define DATA_FRAME							 0
#define REMOTE_FRAME						 1

#define FIFO_0_MESSAGE_PENDING 				 0
#define FIFO_1_MESSAGE_PENDING				 1
#define FIFO_NO_MESSAGE_PENDING				 -1


#define CAN_RESET_BIT_POSITION				 		15
#define REMOTE_TX_REQUEST_BIT_POS			 		 1
#define TRANSMIT_MAILBOX_REQUEST_BIT_POS	 		 0
#define INIT_REQUEST_BIT_POS				         0
#define SLEEP_REQUEST_BIT_POS				 		 1
#define INIT_ACK_BIT_POSITION				 		 0
#define SLEEP_ACK_BIT_POSITION				 		 1
#define STANDARD_ID_BIT_POSITION			 		21
#define EXTENDED_ID_BIT_POSITION			  		 3
#define RELEASE_FIFO_MAILBOX_BIT_POS		  		 5
#define NO_AUTOMATIC_RE_TRANSMISSION_BIT_POS		 4
#define ERROR_INTERRUPT_BIT_POS						15
#define TX_MAILBOX_EMPTY_INTERRUPT_BIT_POS			 0
#define FILTER_INIT_MODE_BIT_POS					 0
#define FIFO_0_MESSAGE_PENDING_INTERRUPT_BIT_POS     1
#define FIFO_1_MESSAGE_PENDING_INTERRUPT_BIT_POS     4


#define NO_AUTOMATIC_RE_TRANSMISSION_EN 		1
#define NO_AUTOMATIC_RE_TRANSMISSION_DI	 		0


#define ERROR_INTERRUPT_DI						0
#define ERROR_INTERRUPT_EN						1


#define TX_MAILBOX_EMPTY_INTERRUPT_DI			2
#define TX_MAILBOX_EMPTY_INTERRUPT_EN			3


#define FIFO_0_MESSAGE_PENDING_INTERRUPT_DI		4
#define FIFO_0_MESSAGE_PENDING_INTERRUPT_EN		5


#define FIFO_1_MESSAGE_PENDING_INTERRUPT_DI 	6
#define FIFO_1_MESSAGE_PENDING_INTERRUPT_EN		7


/*
 * FBM bit in FM1R register
 */
#define IDENTIFIER_MASK_MODE					0
#define IDENTIFIER_LIST_MODE					1

/*
 *  FSC bit in FS1R register
 */
#define SCALE_16_BIT							0
#define SCALE_32_BIT							1

/*
 *	FFA bit in FAA1R register
 */
#define FILTER_ASSIGN_TO_FIFO_0					0
#define FILTER_ASSIGN_TO_FIFO_1					1

/*
 * FACT bit in FA1R register
 */
#define FILTER_ACTIVE							0
#define FILTER_DEACTIVE							1



typedef enum{
	FILTER_1 = 0,
	FILTER_2    ,
	FILTER_3    ,
	FILTER_4	,
	FILTER_5	,
	FILTER_6	,
	FILTER_7	,
	FILTER_8 	,
	FILTER_9	,
	FILTER_10	,
	FILTER_11 	,
	FILTER_12	,
	FILTER_13	,
	FILTER_14	,
}filter_index_e;


typedef struct{
	uint8_t scale;
	uint8_t mode;
	uint8_t filter_assign_to;
	uint8_t ide;
	uint8_t rtr;
	union{
		uint32_t id_32_bit_1[1];
		uint32_t id_32_bit_2[2];
		uint16_t id_16_bit_2[2];
		uint16_t id_16_bit_4[4];
	}id;
	union{
		uint32_t mask_32_bit_1[1];
		uint32_t mask_32_bit_2[2];
	}mask;
}filter_config_t;

typedef struct{
	uint8_t no_automatic_retransmission_en_di;
	uint8_t tx_mailbox_empty_interrupt_en_di;
	uint8_t fifo_0_message_pending_interrupt_en_di;
	uint8_t fifo_0_full_interrupt_en_di;
	uint8_t fifo_0_overrun_interrupt_en_di;
	uint8_t fifo_1_message_pending_interrupt_en_di;
	uint8_t fifo_1_full_interrupt_en_di;
	uint8_t fifo_1_overrun_interrupt_en_di;
	uint8_t error_interrupt_en_di;
	uint8_t test_mode;
	uint8_t resync_width;
	uint8_t time_segment_2;
	uint8_t time_segment_1;
	uint16_t brp;
	filter_index_e filter_index;
	filter_config_t filter_config[14];
}can_config_t;

typedef union{
	uint8_t data_8[8];
	uint16_t data_16[4];
	uint32_t data_32[2];
}can_data_frame_t;

typedef struct{
	uint8_t ide;
	uint8_t rtr;
	uint32_t id;
	uint8_t length;
	can_data_frame_t data;
}can_tx_frame_t;

typedef struct{
	uint8_t ide;
	uint8_t rtr;
	uint16_t time;
	uint8_t length;
	uint8_t filter_match_index;
	uint32_t id;
	union{
		uint8_t data_8[8];
		uint16_t data_16[4];
		uint32_t data_32[2];
	};
}can_rx_frame_t;

typedef enum{
	CAN_IDEAL,
	CAN_TX_OK,
	CAN_MAILBOX_NOT_EMPTY = -1,
}can_status_t;

typedef void (*can_rx0_callback_t)(const can_rx_frame_t*);
extern can_tx_frame_t can_tx;
extern can_config_t can_configuration;
extern can_rx_frame_t can_rx;


uint8_t hal_can_init(can_config_t* can_config);
uint8_t hal_can_transmit(can_regdef_t* p_can,const uint8_t* p_data,uint8_t length,uint8_t rtr, uint8_t tde, uint32_t id);
uint8_t hal_can_receive(can_regdef_t* p_can);
uint8_t can_init(can_regdef_t* p_can, can_config_t* can_config);
uint8_t can_transmit(can_regdef_t* p_can, can_tx_frame_t* p_can_tx);
void register_rx0_callback(can_rx0_callback_t callback);
uint8_t can_receive_message(can_regdef_t* p_can, uint8_t fifo_message_pending_index);
void enter_init_mode(can_regdef_t* p_can);
void enter_normal_mode(can_regdef_t* p_can);
void set_test_mode(can_regdef_t* p_can,uint8_t test_mode);
void set_bit_timing(can_regdef_t* p_can,uint32_t baudrate);
void interrupt_en_di(can_regdef_t* p_can,uint8_t en_di);
void filter_config(can_regdef_t* p_can,filter_index_e filter_number,filter_config_t* filter_configuration);


#endif /* INC_CAN_H_ */
