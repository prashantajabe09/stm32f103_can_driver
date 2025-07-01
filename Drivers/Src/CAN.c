/*
 * CAN.c
 *
 *  Created on: Jun 5, 2025
 *      Author: PRASHANT AJABE
 */


#include "CAN.h"

can_tx_frame_t can_tx;
can_rx_frame_t can_rx;
can_config_t can_configuration;


can_status_t can_status = CAN_IDEAL;

static can_rx0_callback_t callback_rx0 = 0;


/**
 * @brief Helper Function
 * Exit from the sleep mode
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 *
 * @return    none
 *
 */
static void exit_sleep_mode(can_regdef_t* p_can)
{
	p_can->MCR &= ~(1 << SLEEP_REQUEST_BIT_POS);
	while((p_can->MSR & (1 << SLEEP_ACK_BIT_POSITION)));
}

/**
 * @brief Helper Function
 * Exit from the initialization mode
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 *
 * @return    none
 *
 */
static void exit_init_mode(can_regdef_t* p_can)
{
	p_can->MCR &= ~(1 << INIT_REQUEST_BIT_POS);
	while((p_can->MSR & (1 << INIT_ACK_BIT_POSITION)));
}

/**
 * @brief Helper function to EN/DI error interrupt
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  EN OR DI
 *
 * @return    none
 */
static void error_interrupt_en_di(can_regdef_t* p_can,uint8_t en_di)
{
	if (en_di == ERROR_INTERRUPT_EN)
	{
		p_can->IER |= (1 << ERROR_INTERRUPT_BIT_POS);
	}
	else if(en_di == ERROR_INTERRUPT_DI)
	{
		p_can->IER &= ~(1 << ERROR_INTERRUPT_BIT_POS);
	}
}

/**
 * @brief Helper function to EN/DI AUTO TRANSMISSION
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  EN OR DI
 *
 * @return    none
 *
 */
static void no_art_en_di(can_regdef_t* p_can,uint8_t en_di)
{
	if (en_di == NO_AUTOMATIC_RE_TRANSMISSION_EN)
	{
		p_can->MCR |= (1 << NO_AUTOMATIC_RE_TRANSMISSION_BIT_POS);
	}
	else if(en_di == NO_AUTOMATIC_RE_TRANSMISSION_DI)
	{
		p_can->MCR &= ~(1 << NO_AUTOMATIC_RE_TRANSMISSION_BIT_POS);
	}
}


/**
 * @brief Check which Tx mailbox is empty
 *
 * Check for empty Tx mailbox and return mailbox number.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 *
 * @return Return Tx mailbox number or mailbox Not empty code
 *
 */
static int8_t get_empty_mailbox(can_regdef_t* p_can)
{

	if (p_can->TSR & (1 << TRANSMIT_MAILBOX_0_EMPTY_BIT_POS))
	{
		return TRANSMIT_MAILBOX_0;
	}
	else if (p_can->TSR & (1 << TRANSMIT_MAILBOX_1_EMPTY_BIT_POS))
	{
		return TRANSMIT_MAILBOX_1;
	}
	else if (p_can->TSR & (1 << TRANSMIT_MAILBOX_2_EMPTY_BIT_POS))
	{
		return TRANSMIT_MAILBOX_2;
	}
	else
	{
		return TRANSMIT_MAILBOX_NOT_EMPTY;
	}
}

/**
 * @brief Set Tx Request
 * set the Tx request for specified mailbox.
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] mailbox index.
 *
 * @return void.
 *
 */
static void send_transmit_mailbox_request(can_regdef_t* p_can, uint8_t mailbox_index)
{
	p_can->canx_tx_mailbox[mailbox_index].TIxR |= (1 << TRANSMIT_MAILBOX_REQUEST_BIT_POS);
}

/**
 * @brief Check if any data present in FIFO.
 *
 * Checks if there is any data present in FIFO. and retruns the FIFO number.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 *
 * @return FIFO Number
 *
 */
static uint8_t check_for_data(can_regdef_t* p_can)
{
	if ( (p_can->RFxR[0] & 0x3) != 0x0)
	{
		return FIFO_0_MESSAGE_PENDING;
	}
	else if ( (p_can->RFxR[1] & 0x3) != 0x0)
	{
		return FIFO_1_MESSAGE_PENDING;
	}
	else
	{
		return FIFO_NO_MESSAGE_PENDING;
	}
}

/**
 * @brief Reads RTR value from FIFO register.
 *
 *  Read RTR value from the FIFO register and store in global structure.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] FIFO number (FIFO index)
 *
 * @return void
 *
 */
static void read_rtr(can_regdef_t* p_can,uint8_t fifo_index)
{
	can_rx.rtr = ((p_can->can_rx_mailbox[fifo_index].RIxR >> 1) & 0x01);
}

/**
 * @brief Reads IDE value from FIFO register.
 *
 *  Read IDE value from the FIFO register and store in global structure.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] FIFO number (FIFO index)
 * @return void
 *
 */
static void read_ide(can_regdef_t* p_can,uint8_t fifo_index)
{
	can_rx.ide = ((p_can->can_rx_mailbox[fifo_index].RIxR >> 1) & 0x02);
}

/**
 * @brief Reads ID value from FIFO register.
 *
 *  Read ID value from the FIFO register and store in global structure.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] FIFO number (FIFO index)
 *
 * @return void
 */
static void read_id(can_regdef_t* p_can,uint8_t fifo_index)
{
	if (can_rx.ide == STANDARD_FRAME)
	{
		can_rx.id = (( p_can->can_rx_mailbox[fifo_index].RIxR >> 21) & 0x7FF);
	}
	else if (can_rx.ide == EXTENDED_FRAME)
	{
		can_rx.id = (( p_can->can_rx_mailbox[fifo_index].RIxR >> 3) & 0x1FFFFFFF);
	}
}

/**
 * @brief Reads length value from FIFO register.
 *
 *  Read Length value from the FIFO register and store in global structure.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] FIFO number (FIFO index)
 *
 * @return void
 */
static void read_length(can_regdef_t* p_can,uint8_t fifo_index)
{
	can_rx.length = (p_can->can_rx_mailbox[fifo_index].RDTxR & 0xF);
}

/**
 * @brief Reads time stamp value from FIFO register.
 *
 *  Read time stamp value from the FIFO register and store in global structure.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] FIFO number (FIFO index)
 *
 * @return void
 */
static void read_time_stamp(can_regdef_t* p_can,uint8_t fifo_index)
{
	can_rx.time = (( p_can->can_rx_mailbox[fifo_index].RDTxR >> 16) & 0xFFFF);
}

/**
 * @brief Reads filter match Index value from FIFO register.
 *
 *  Read filter match Index value from the FIFO register and store in global structure.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] FIFO number (FIFO index)
 *
 * @return void
 */
static void read_filter_match_index(can_regdef_t* p_can,uint8_t fifo_index)
{
	can_rx.filter_match_index = (( p_can->can_rx_mailbox[fifo_index].RDTxR >> 8) & 0xFF);
}

/**
 * @brief Reads data value from FIFO register.
 *
 *  Read data value from the FIFO register and store in global structure.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] FIFO number (FIFO index)
 * @return void
 *
 */

static void read_data(can_regdef_t* p_can,uint8_t fifo_index)
{
	can_rx.data_32[0] = p_can->can_rx_mailbox[fifo_index].RDLxR;
	can_rx.data_32[1] = p_can->can_rx_mailbox[fifo_index].RDHxR;
}

/**
 * @brief Releases the FIFO after data read
 *
 *  Releases the FIFO after received data frame.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in] FIFO number (FIFO index)
 *
 * @return void
 *
 */
static void release_fifo(can_regdef_t* p_can,uint8_t fifo_index)
{
	p_can->RFxR[fifo_index] |= (1 << RELEASE_FIFO_MAILBOX_BIT_POS);
}

/**
 * @brief User will fill configuration structure as per requirement and
 * call initialization function to configure the peripheral
 *
 * @param[in]  global configuration structure.
 *
 * @return    none
 */
uint8_t hal_can_init(can_config_t* can_config)
{
	can_config->no_automatic_retransmission_en_di = NO_AUTOMATIC_RE_TRANSMISSION_DI;
	can_config->error_interrupt_en_di = ERROR_INTERRUPT_DI;
	can_config->test_mode = LOOPBACK_TEST_MODE_EN;
	can_config->tx_mailbox_empty_interrupt_en_di = TX_MAILBOX_EMPTY_INTERRUPT_DI;
	can_config->fifo_0_message_pending_interrupt_en_di = FIFO_0_MESSAGE_PENDING_INTERRUPT_EN;
	can_config->filter_index = FILTER_3;
	can_config->filter_config[can_config->filter_index].id.id_16_bit_2[0] = 0x27;
	can_config->filter_config[can_config->filter_index].id.id_16_bit_2[1] = 0x27;

	can_config->filter_config[can_config->filter_index].scale = SCALE_16_BIT;
	can_config->filter_config[can_config->filter_index].mode = IDENTIFIER_MASK_MODE;
	can_config->filter_config[can_config->filter_index].filter_assign_to = FILTER_ASSIGN_TO_FIFO_0;
	can_config->filter_config[can_config->filter_index].ide = STANDARD_FRAME;
	can_config->filter_config[can_config->filter_index].rtr = DATA_FRAME;
	can_config->filter_config[can_config->filter_index].mask.mask_32_bit_2[0] = 0xFE000000;
	can_config->filter_config[can_config->filter_index].mask.mask_32_bit_2[1] = 0xFE000000;
	can_init(CAN1,&can_configuration);

}

/**
 * @brief Call other helper function to initialize the CAN peripheral based on configuration structure.
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  global configuration structure.
 * @return    none
 *
 */
uint8_t can_init(can_regdef_t* p_can, can_config_t* can_config)
{
	enter_init_mode(p_can);

	CAN1->MCR &= ~(1 << 16);

	set_bit_timing(p_can,250000);

	set_test_mode(p_can,can_config->test_mode);

	error_interrupt_en_di(p_can,can_config->error_interrupt_en_di);

	no_art_en_di(p_can,can_config->no_automatic_retransmission_en_di);

	interrupt_en_di(p_can,can_config->tx_mailbox_empty_interrupt_en_di);

	interrupt_en_di(p_can,can_config->fifo_0_message_pending_interrupt_en_di);

	filter_config(p_can,can_config->filter_index,&(can_config->filter_config[can_config->filter_index]));

	enter_normal_mode(p_can);

}

/**
 * @brief Function to Enter normal mode after either sleep mode or init mode
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 *
 * @return    none
 *
 */
void enter_normal_mode(can_regdef_t* p_can)
{
	exit_init_mode(p_can);
	exit_sleep_mode(p_can);
}

/**
 * @brief EN/DI Interrupts and set in NVIC
 *
 * EN/DI Tx empty and FIFO message pending related interrupt
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  EN OR DI
 *
 * @return    none
 *
 */
void interrupt_en_di(can_regdef_t* p_can,uint8_t en_di)
{
	if (en_di == TX_MAILBOX_EMPTY_INTERRUPT_EN)
	{
		p_can->IER |= (1 << TX_MAILBOX_EMPTY_INTERRUPT_BIT_POS);
		nvic_en_irq(CAN1_TX_IRQn);
	}
	else if(en_di == TX_MAILBOX_EMPTY_INTERRUPT_DI)
	{
		p_can->IER &= ~(1 << TX_MAILBOX_EMPTY_INTERRUPT_BIT_POS);
	}
	else if (en_di == FIFO_0_MESSAGE_PENDING_INTERRUPT_EN)
	{
		p_can->IER |= (1 << FIFO_0_MESSAGE_PENDING_INTERRUPT_BIT_POS);
		nvic_en_irq(CAN1_RX_0_IRQn);
	}
	else if(en_di == FIFO_0_MESSAGE_PENDING_INTERRUPT_DI)
	{
		p_can->IER &= ~(1 << FIFO_0_MESSAGE_PENDING_INTERRUPT_BIT_POS);
	}
	else if (en_di == FIFO_1_MESSAGE_PENDING_INTERRUPT_EN)
	{
		p_can->IER |= (1 << FIFO_1_MESSAGE_PENDING_INTERRUPT_BIT_POS);
		nvic_en_irq(CAN1_RX_1_IRQn);
	}
	else if(en_di == FIFO_1_MESSAGE_PENDING_INTERRUPT_DI)
	{
		p_can->IER &= ~(1 << FIFO_1_MESSAGE_PENDING_INTERRUPT_BIT_POS);
	}
}

/**
 * @brief configure filters
 * configure the filter based on the configuration in the configuration structure.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  filter number
 * @param[in]  filter configuration structure pointer.
 *
 * @return    none
 */
void filter_config(can_regdef_t* p_can,filter_index_e filter_number,filter_config_t* filter_configuration)
{

	p_can->FMR |= (1 << FILTER_INIT_MODE_BIT_POS);
	//mode
	if (filter_configuration->mode == IDENTIFIER_LIST_MODE)
	{
		p_can->FM1R |= (1 << filter_number);
	}
	else if (filter_configuration->mode == IDENTIFIER_MASK_MODE)
	{
		p_can->FM1R &= ~(1 << filter_number);
	}

	//scale
	if (filter_configuration->scale == SCALE_32_BIT)
	{
		p_can->FS1R |= (1 << filter_number);
	}
	else if (filter_configuration->scale == SCALE_32_BIT)
	{
		p_can->FS1R |= (1 << filter_number);
	}

	//filter assignement to FIFO
	if (filter_configuration->filter_assign_to == FILTER_ASSIGN_TO_FIFO_1)
	{
		p_can->FFA1R |= (1 << filter_number);
	}
	else if (filter_configuration->filter_assign_to == FILTER_ASSIGN_TO_FIFO_0)
	{
		p_can->FFA1R &= ~(1 << filter_number);
	}


	if (filter_configuration->scale == SCALE_32_BIT && filter_configuration->mode == IDENTIFIER_LIST_MODE)
	{
		// condition for standard and extented frame
		if(filter_configuration->ide == STANDARD_FRAME)
		{
			p_can->FxRi[filter_number*2].u32 = filter_configuration->id.id_32_bit_2[0] << 21 | (filter_configuration->ide << 2) | (filter_configuration->rtr << 1);
			p_can->FxRi[filter_number*2 + 1].u32 = filter_configuration->id.id_32_bit_2[1] << 21 | (filter_configuration->ide << 2) | (filter_configuration->rtr << 1);
		}
		else if (filter_configuration->ide == EXTENDED_FRAME)
		{
			p_can->FxRi[filter_number*2].u32 = filter_configuration->id.id_32_bit_2[0] << 3 | (filter_configuration->ide << 2) | (filter_configuration->rtr << 1);
			p_can->FxRi[filter_number*2 + 1].u32 = filter_configuration->id.id_32_bit_2[1] << 3 | (filter_configuration->ide << 2) | (filter_configuration->rtr << 1);
		}
	}
	else if (filter_configuration->scale == SCALE_32_BIT && filter_configuration->mode == IDENTIFIER_MASK_MODE)
	{
		// condition for standard and extended frame.
		if(filter_configuration->ide == STANDARD_FRAME)
		{
			p_can->FxRi[filter_number*2].u32 = filter_configuration->id.id_32_bit_2[0] << 21 | (filter_configuration->ide << 2) | (filter_configuration->rtr << 1);
			p_can->FxRi[filter_number*2 + 1].u32 = filter_configuration->mask.mask_32_bit_1[0]  | (filter_configuration->ide << 2) | (filter_configuration->rtr << 1);
		}
		else if (filter_configuration->ide == EXTENDED_FRAME)
		{
			p_can->FxRi[filter_number*2].u32 = filter_configuration->id.id_32_bit_2[0] << 3 | (filter_configuration->ide << 2) | (filter_configuration->rtr << 1);
			p_can->FxRi[filter_number*2 + 1].u32 = filter_configuration->mask.mask_32_bit_1[0] | (filter_configuration->ide << 2) | (filter_configuration->rtr << 1);
		}
	}
	else if (filter_configuration->scale == SCALE_16_BIT && filter_configuration->mode == IDENTIFIER_LIST_MODE)
	{
		// restricted to standard only.
		p_can->FxRi[filter_number*2].u32 = (filter_configuration->id.id_16_bit_4[1] << 21 | (filter_configuration->ide << 20) | (filter_configuration->rtr << 19))\
											| (filter_configuration->id.id_16_bit_4[0] << 5 | (filter_configuration->ide << 4) | (filter_configuration->rtr << 3));

		p_can->FxRi[filter_number*2 + 1].u32 = (filter_configuration->id.id_16_bit_4[3] << 21 | (filter_configuration->ide << 20) | (filter_configuration->rtr << 19))\
												| (filter_configuration->id.id_16_bit_4[2] << 5 | (filter_configuration->ide << 4) | (filter_configuration->rtr << 3));
	}
	else if (filter_configuration->scale == SCALE_16_BIT && filter_configuration->mode == IDENTIFIER_MASK_MODE)
	{
		p_can->FxRi[filter_number*2].u32 = filter_configuration->mask.mask_32_bit_2[0] | (filter_configuration->id.id_16_bit_2[0] << 5) | (filter_configuration->ide << 4) | (filter_configuration->rtr << 3);
		p_can->FxRi[filter_number*2 + 1].u32 = filter_configuration->mask.mask_32_bit_2[1] | (filter_configuration->id.id_16_bit_2[1] << 5) | (filter_configuration->ide << 4) | (filter_configuration->rtr << 3);

	}

	//activate the filter.
	p_can->FA1R |= (1 << filter_number);

	//exit from initialization mode.
	p_can->FMR &= ~(1 << FILTER_INIT_MODE_BIT_POS);
}

/**
 * @brief set the test mode as per the input
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  test mode (LOOPBACK OR SILENT)
 *
 * @return    none
 *
 */
void set_test_mode(can_regdef_t* p_can,uint8_t test_mode)
{
	if (test_mode == LOOPBACK_TEST_MODE_EN)
	{
		p_can->BTR |= (1 << LOOPBACK_TEST_MODE_BIT_POSITION);
	}
	else if (test_mode == LOOPBACK_TEST_MODE_DI)
	{
		p_can->BTR &= ~(1 << LOOPBACK_TEST_MODE_BIT_POSITION);
	}
	else if (test_mode == SILENT_TEST_MODE_EN)
	{
		p_can->BTR |= (1 << SILENT_TEST_MODE_BIT_POSITION);
	}
	else if (test_mode == SILENT_TEST_MODE_DI)
	{
		p_can->BTR &= ~(1 << SILENT_TEST_MODE_BIT_POSITION);
	}
}

/**
 * @brief sequence to enter the initialization mode.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 *
 * @return    none
 *
 */
void enter_init_mode(can_regdef_t* p_can)
{
	exit_sleep_mode(p_can);
	CAN1->MCR |= (1 << INIT_REQUEST_BIT_POS);
	while(!(p_can->MSR & (1 << INIT_ACK_BIT_POSITION)));
}

/**
 * @brief Set BTR register values
 * Set BTR register values based on the input of the baudrate
 * In this function considered Number of TQ as 16 with TS1 = 13, TS2 = 2 and SJW = 1.
 * BRP calculated from the clock frequency of the bus and number of TQ.
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  baudrate

 * @return    none
 *
 */
void set_bit_timing(can_regdef_t* p_can,uint32_t baudrate)
{
	uint16_t brp;
	brp = (uint16_t)( (apb_get_clock(APB1) / (baudrate * 16) ) - 1);
	p_can->BTR = 0x00000000;
	p_can->BTR |= (brp << 0);
	p_can->BTR |= 0xC << 16;
	p_can->BTR |= 0x1 << 20;
	p_can->BTR |= 0x0 << 24;
}

/**
 * @brief HAL level CAN transmit function
 * Sets the global CAN TX frame as per the parameter.
 * Call the driver level transmit function to set mailbox as per the CAN tx frame.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  data pointer
 * @param[in] length of the data
 * @param[in] rtr
 * @param[in] id type
 * @param[in] ID
 *
 * @return    none
 *
 */
uint8_t hal_can_transmit(can_regdef_t* p_can,const uint8_t* p_data,uint8_t length,uint8_t rtr, uint8_t id_type, uint32_t id)
{
	can_tx.ide = id_type;
	can_tx.rtr = rtr;
	can_tx.id = id;
	can_tx.length = length;
	for (uint8_t i = 0; i < length;i++)
	{
		can_tx.data.data_8[i] = *p_data++;

	}
	can_transmit(p_can,&can_tx);
}

/**
 * @brief driver level can trnasmit function
 * This function checks for the Empty mailbox.
 * Set can Tx frame in the mailbox as per the RTR and IDE.
 * set request for mailbox transmit.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[in]  Global CAN Tx frame
 *
 * @return status of mailbox
 *
 */
uint8_t can_transmit(can_regdef_t* p_can, can_tx_frame_t* p_can_tx)
{
	int8_t mailbox_index;

	while((mailbox_index = get_empty_mailbox(p_can)) == TRANSMIT_MAILBOX_NOT_EMPTY);

	if (mailbox_index != TRANSMIT_MAILBOX_NOT_EMPTY)
	{
		p_can->canx_tx_mailbox[mailbox_index].TIxR  = 0x00000000;
		p_can->canx_tx_mailbox[mailbox_index].TDTxR = 0x00000000;
		if (p_can_tx->ide == STANDARD_FRAME)
		{
			p_can->canx_tx_mailbox[mailbox_index].TIxR &= ~(1 << 2);
			p_can->canx_tx_mailbox[mailbox_index].TIxR |= p_can_tx->id << STANDARD_ID_BIT_POSITION;
		}
		else if(p_can_tx->ide ==  EXTENDED_FRAME)
		{
			p_can->canx_tx_mailbox[mailbox_index].TIxR |= (1 << 2);
			p_can->canx_tx_mailbox[mailbox_index].TIxR |= p_can_tx->id << EXTENDED_ID_BIT_POSITION;
		}

		if (p_can_tx->rtr == DATA_FRAME)
		{
			p_can->canx_tx_mailbox[mailbox_index].TIxR &= ~(1 << 1);
		}
		else if (p_can_tx->rtr == REMOTE_FRAME)
		{
			p_can->canx_tx_mailbox[mailbox_index].TIxR |= (1 << 1);
		}

		p_can->canx_tx_mailbox[mailbox_index].TDTxR |= ( p_can_tx->length << 0);
		p_can->canx_tx_mailbox[mailbox_index].TDLxR = p_can_tx->data.data_32[0];
		p_can->canx_tx_mailbox[mailbox_index].TDHxR = p_can_tx->data.data_32[1];
	}
	else
	{

		return TRANSMIT_MAILBOX_NOT_EMPTY;
	}

	send_transmit_mailbox_request(p_can,mailbox_index);

}

/**
 * @brief Abstraction layer for CAN_Receive function
 *
 *Check for which FIFO have data and accordingly call can_receive_message() with respective FIFO number.
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 *
 * @return Return Receive Status
 *
 */
uint8_t hal_can_receive(can_regdef_t* p_can)
{
	uint8_t fifo_message_pending_index = check_for_data(p_can);

	if (fifo_message_pending_index != FIFO_NO_MESSAGE_PENDING)
	{
		return (can_receive_message(p_can,fifo_message_pending_index));
	}
	else
	{
		return FIFO_NO_MESSAGE_PENDING;
	}
}

/**
 * @brief Read a CAN message from a specified FIFO.
 *
 * This function reads ID, Length, Filter Match Index and Data from selected FIFO.
 * Stores the data to global Rx structure and Releases the the FIFO after read
 *
 * @param[in]  Pointer To CAN peripheral. (e.g. CAN1..etc)
 * @param[out] FIFO number to read from (0 or 1)
 *
 * @return RX_DONE (1)
 *
 */
uint8_t can_receive_message(can_regdef_t* p_can, uint8_t fifo_message_pending_index)
{

		read_rtr(p_can,fifo_message_pending_index);

		read_ide(p_can,fifo_message_pending_index);

		read_id(p_can,fifo_message_pending_index);

		read_length(p_can,fifo_message_pending_index);

		read_time_stamp(p_can,fifo_message_pending_index);

		read_filter_match_index(p_can,fifo_message_pending_index);

		read_data(p_can,fifo_message_pending_index);

		release_fifo(p_can,fifo_message_pending_index);

		return RX_DONE;

}

/**
 * @brief Register callback.
 *
 *  Register callback function.
 *
 * @param[in]  Function name
 *
 * @return void
 *
 */
void register_rx0_callback(can_rx0_callback_t callback)
{
	callback_rx0 = callback;
}

/**
 * @brief RX0 interrrupt handler.
 *
 *  handles the interrupts generated by FIFO FUll, FIFO Overrun and data received interrupt.
 *
 * @param[in]  Function name
 *
 * @return void
 *
 */
void USB_LP_CAN_RX0_IRQHandler(void)
{
	uint8_t temp = 0;
	// FIFO FULL

	// FIFO OVERRUN

	// FIFO message received
	if (check_for_data(CAN1) == FIFO_0_MESSAGE_PENDING)
	{
		can_receive_message(CAN1,FIFO_0_MESSAGE_PENDING); // reads the message.
		if (callback_rx0)
		{
			callback_rx0(&can_rx);
		}
	}

}


