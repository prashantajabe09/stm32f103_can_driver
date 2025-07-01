/*
 * can_test.c
 *
 *  Created on: Jun 25, 2025
 *      Author: PRASHANT AJABE
 */



#include "can_test.h"

/**
 * @brief Error Handler.
 *
 *  This Function is called to print file name and line number where test failed
 *
 * @param[in]  File
 * @param[in] line
 *
 * @return none
 */
void error_handler(const char *file, int line) {

	uart_send_string("assertion failed at: ");

	uart_send_string(file);

	uart_send_string(" : ");

	uart_sent_uint((uint32_t)line);

    while (1); // freeze
}

/**
 * @brief Test BTR register write.
 *
 *  This test case check for proper register write BTR as per the baudrate.
 *
 * @param[in]  CAN peripheral Address
 * @param[in] Baudrate
 * @param[in] expected BTR
 *
 * @return none
 */
uint8_t test_can_btr(can_regdef_t* p_can,uint32_t baudrate,uint32_t expected_btr)
{
	static counter = 0;
	counter++;
	enter_init_mode(p_can);
	set_bit_timing(p_can,baudrate);
	enter_normal_mode(p_can);
	ASSERT_TEST((CAN1->BTR & 0x3FFFFFF) == expected_btr);
	uart_send_string("BTR TEST CASE PASS...!");
	uart_sent_uint(counter);
	uart_send_string("\r\n");
}

/**
 * @brief Test 32-bit ID mask mode.
 *
 *  This test case check for proper register write of ID and mask for 32-bit scale and ID Mask mode.
 *  This is checked for Standard Frame.
 *
 * @param[in]  filter number
 * @param[in] ID
 * @param[in] Mask
 * @param[in] filter_assign_to (filter assign to which fifo)
 * @param[in] ide (id type)
 * @param[in] rtr
 *
 * @return none
 */
uint8_t test_can_filter_32_bit_id_mask_mode(filter_index_e filter_index,uint32_t id, uint32_t mask, \
		uint8_t filter_assign_to, uint8_t ide, uint8_t rtr)
{
	can_configuration.filter_index = filter_index;
	can_configuration.filter_config[can_configuration.filter_index].id.id_32_bit_1[0] = id;
	can_configuration.filter_config[can_configuration.filter_index].scale = SCALE_32_BIT;
	can_configuration.filter_config[can_configuration.filter_index].mode = IDENTIFIER_MASK_MODE;
	can_configuration.filter_config[can_configuration.filter_index].filter_assign_to = filter_assign_to;
	can_configuration.filter_config[can_configuration.filter_index].ide = ide;
	can_configuration.filter_config[can_configuration.filter_index].rtr = rtr;
	can_configuration.filter_config[can_configuration.filter_index].mask.mask_32_bit_1[0] = mask;

	enter_init_mode(CAN1);
	filter_config(CAN1,can_configuration.filter_index,&(can_configuration.filter_config[can_configuration.filter_index]));
	if (can_configuration.filter_config[can_configuration.filter_index].ide == STANDARD_FRAME)
	{

		ASSERT_TEST(CAN1->FxRi[can_configuration.filter_index * 2].u32 >> 21 == id);
		ASSERT_TEST(CAN1->FxRi[(can_configuration.filter_index * 2) + 1].u32 == mask);
		uart_send_string("Filter 32 bit ID Mask standard Frame TEST CASE PASS...! \r\n");

	}
	else if (can_configuration.filter_config[can_configuration.filter_index].ide == EXTENDED_FRAME)
	{
		ASSERT_TEST(CAN1->FxRi[can_configuration.filter_index * 2].u32 >> 3 == id);
		ASSERT_TEST(CAN1->FxRi[(can_configuration.filter_index * 2) + 1].u32 == mask);
		uart_send_string("Filter 32 bit ID Mask extended Frame TEST CASE PASS...! \r\n");
	}

	enter_normal_mode(CAN1);
}

/**
 * @brief Test 16-bit ID List mode.
 *
 *  This test case check for proper register write of IDs for 16-bit scale and ID List mode.
 *  This is checked for Standard Frame.
 *
 * @param[in]  filter number
 * @param[in] IDs pointer
 * @param[in] filter_assign_to (filter assign to which fifo)
 * @param[in] rtr
 *
 * @return none
 */
uint8_t test_can_filter_16_bit_id_list_mode(filter_index_e filter_index,uint16_t* id, \
		uint8_t filter_assign_to, uint8_t rtr)
{
	uint16_t* p_id_list = id;
	can_configuration.filter_index = filter_index;
	can_configuration.filter_config[can_configuration.filter_index].id.id_16_bit_4[0] = *p_id_list++;
	can_configuration.filter_config[can_configuration.filter_index].id.id_16_bit_4[1] = *p_id_list++;
	can_configuration.filter_config[can_configuration.filter_index].id.id_16_bit_4[2] = *p_id_list++;
	can_configuration.filter_config[can_configuration.filter_index].id.id_16_bit_4[3] = *p_id_list;
	can_configuration.filter_config[can_configuration.filter_index].scale = SCALE_16_BIT;
	can_configuration.filter_config[can_configuration.filter_index].mode = IDENTIFIER_LIST_MODE;
	can_configuration.filter_config[can_configuration.filter_index].filter_assign_to = filter_assign_to;
	can_configuration.filter_config[can_configuration.filter_index].ide = STANDARD_FRAME;
	can_configuration.filter_config[can_configuration.filter_index].rtr = rtr;

	enter_init_mode(CAN1);
	filter_config(CAN1,can_configuration.filter_index,&(can_configuration.filter_config[can_configuration.filter_index]));
	enter_normal_mode(CAN1);

	p_id_list = id;

	ASSERT_TEST(( CAN1->FxRi[can_configuration.filter_index * 2].u32 >> 5 & 0xFFFF) == *p_id_list++);
	uart_send_string("Filter 16 bit ID list ID_1 TEST CASE PASS...! \r\n");

	ASSERT_TEST((CAN1->FxRi[can_configuration.filter_index * 2].u32 >> 21 & 0xFFFF) == *p_id_list++);
	uart_send_string("Filter 16 bit ID list ID_2 TEST CASE PASS...! \r\n");

	ASSERT_TEST((CAN1->FxRi[(can_configuration.filter_index * 2) + 1].u32 >> 5 & 0xFFFF) == *p_id_list++);
	uart_send_string("Filter 16 bit ID list ID_3 TEST CASE PASS...! \r\n");

	ASSERT_TEST((CAN1->FxRi[(can_configuration.filter_index * 2) + 1].u32 >> 21 & 0xFFFF) == *p_id_list);
	uart_send_string("Filter 16 bit ID list ID_4 TEST CASE PASS...! \r\n");


}

/**
 * @brief Test 16-bit ID mask mode.
 *
 *  This test case check for proper register write of ID and mask for 16-bit scale and ID Mask mode.
 *  This is checked for Standard Frame.
 *
 * @param[in]  filter number
 * @param[in] IDs pointer
 * @param[in] Mask pointer
 * @param[in] filter_assign_to (filter assign to which fifo)
 * @param[in] rtr
 *
 * @return none
 */
uint8_t test_can_filter_16_bit_id_mask_mode(filter_index_e filter_index,uint16_t* id, uint32_t* mask, \
		uint8_t filter_assign_to, uint8_t rtr)
{
	uint16_t* p_id_list = id;
	uint32_t* p_mask_list = mask;
	can_configuration.filter_index = filter_index;
	can_configuration.filter_config[can_configuration.filter_index].id.id_16_bit_2[0] = *p_id_list++;
	can_configuration.filter_config[can_configuration.filter_index].id.id_16_bit_2[1] = *p_id_list;
	can_configuration.filter_config[can_configuration.filter_index].mask.mask_32_bit_2[0] = *p_mask_list++;
	can_configuration.filter_config[can_configuration.filter_index].mask.mask_32_bit_2[1] = *p_mask_list;
	can_configuration.filter_config[can_configuration.filter_index].scale = SCALE_16_BIT;
	can_configuration.filter_config[can_configuration.filter_index].mode = IDENTIFIER_MASK_MODE;
	can_configuration.filter_config[can_configuration.filter_index].filter_assign_to = filter_assign_to;
	can_configuration.filter_config[can_configuration.filter_index].ide = STANDARD_FRAME;
	can_configuration.filter_config[can_configuration.filter_index].rtr = rtr;

	enter_init_mode(CAN1);
	filter_config(CAN1,can_configuration.filter_index,&(can_configuration.filter_config[can_configuration.filter_index]));
	enter_normal_mode(CAN1);

	p_id_list = id;
	p_mask_list = mask;

	ASSERT_TEST(( CAN1->FxRi[can_configuration.filter_index * 2].u32 >> 5 & 0xFFFF) == *p_id_list++);
	uart_send_string("Filter 16 bit ID Mask ID_1 TEST CASE PASS...! \r\n");

	ASSERT_TEST(( CAN1->FxRi[can_configuration.filter_index * 2].u32  & 0xFFFF0000) == *p_mask_list++);
	uart_send_string("Filter 16 bit ID Mask mask_1 TEST CASE PASS...! \r\n");


	ASSERT_TEST((CAN1->FxRi[(can_configuration.filter_index * 2) + 1].u32 >> 5 & 0xFFFF) == *p_id_list);
	uart_send_string("Filter 16 bit ID Mask ID_2 TEST CASE PASS...! \r\n");

	ASSERT_TEST((CAN1->FxRi[(can_configuration.filter_index * 2) + 1].u32 & 0xFFFF0000) == *p_mask_list);
	uart_send_string("Filter 16 bit ID Mask mask_2 TEST CASE PASS...! \r\n");
}

/**
 * @brief Test 32-bit ID list mode.
 *
 *  This test case check for proper register write of ID for 32-bit scale and ID list mode
 *
 * @param[in]  filter number
 * @param[in] IDs
 * @param[in] Mask
 * @param[in] filter_assign_to (filter assign to which fifo)
 * @param[in] ide (id type)
 * @param[in] rtr
 *
 * @return none
 */
uint8_t test_can_filter_32_bit_id_list_mode(filter_index_e filter_index,uint32_t id, uint32_t mask, \
						uint8_t filter_assign_to, uint8_t ide, uint8_t rtr)
{
	can_configuration.filter_index = filter_index;
	can_configuration.filter_config[can_configuration.filter_index].id.id_32_bit_2[0] = id;
	can_configuration.filter_config[can_configuration.filter_index].id.id_32_bit_2[1] = id;
	can_configuration.filter_config[can_configuration.filter_index].scale = SCALE_32_BIT;
	can_configuration.filter_config[can_configuration.filter_index].mode = IDENTIFIER_LIST_MODE;
	can_configuration.filter_config[can_configuration.filter_index].filter_assign_to = filter_assign_to;
	can_configuration.filter_config[can_configuration.filter_index].ide = ide;
	can_configuration.filter_config[can_configuration.filter_index].rtr = rtr;
	can_configuration.filter_config[can_configuration.filter_index].mask.mask_32_bit_2[0] = mask;
	can_configuration.filter_config[can_configuration.filter_index].mask.mask_32_bit_2[1] = mask;
	enter_init_mode(CAN1);
	filter_config(CAN1,can_configuration.filter_index,&(can_configuration.filter_config[can_configuration.filter_index]));
	if (can_configuration.filter_config[can_configuration.filter_index].ide == STANDARD_FRAME)
	{

		ASSERT_TEST(CAN1->FxRi[can_configuration.filter_index * 2].u32 >> 21 == id);
		uart_send_string("Filter 32 bit ID list standard Frame 1 TEST CASE PASS...! \r\n");
		ASSERT_TEST(CAN1->FxRi[(can_configuration.filter_index * 2) + 1].u32 >> 21 == id);
		uart_send_string("Filter 32 bit ID list standard Frame 2 TEST CASE PASS...! \r\n");

	}
	else if (can_configuration.filter_config[can_configuration.filter_index].ide == EXTENDED_FRAME)
	{
		ASSERT_TEST(CAN1->FxRi[can_configuration.filter_index * 2].u32 >> 3 == id);
		uart_send_string("Filter 32 bit ID list extended Frame 1 TEST CASE PASS...! \r\n");
		ASSERT_TEST(CAN1->FxRi[(can_configuration.filter_index * 2) + 1].u32 >> 3 == id);
		uart_send_string("Filter 32 bit ID list extended Frame 2 TEST CASE PASS...! \r\n");
	}

	enter_normal_mode(CAN1);

}

/**
 * @brief CAN initialization to Test Loop Back Mode.
 *
 *  Initialize the CAN peripheral for LoopBack Test.
 *
 * @param[in]  CAN Peripheral pointer
 *
 * @return status
 *
 */

void test_can_init_loopback_mode(can_regdef_t* p_can)
{
	enter_init_mode(CAN1);

	set_bit_timing(p_can,250000);

	set_test_mode(p_can,LOOPBACK_TEST_MODE_EN);

	interrupt_en_di(p_can,FIFO_0_MESSAGE_PENDING_INTERRUPT_DI);

	can_configuration.filter_index = FILTER_3;
	can_configuration.filter_config[can_configuration.filter_index].id.id_16_bit_2[0] = 0x27;
	can_configuration.filter_config[can_configuration.filter_index].id.id_16_bit_2[1] = 0x27;
	can_configuration.filter_config[can_configuration.filter_index].scale = SCALE_16_BIT;
	can_configuration.filter_config[can_configuration.filter_index].mode = IDENTIFIER_MASK_MODE;
	can_configuration.filter_config[can_configuration.filter_index].filter_assign_to = FILTER_ASSIGN_TO_FIFO_0;
	can_configuration.filter_config[can_configuration.filter_index].ide = STANDARD_FRAME;
	can_configuration.filter_config[can_configuration.filter_index].rtr = DATA_FRAME;
	can_configuration.filter_config[can_configuration.filter_index].mask.mask_32_bit_2[0] = 0xFE000000;
	can_configuration.filter_config[can_configuration.filter_index].mask.mask_32_bit_2[1] = 0xFE000000;
	filter_config(p_can,can_configuration.filter_index,&(can_configuration.filter_config[can_configuration.filter_index]));

	enter_normal_mode(CAN1);
}

/**
 * @brief Test the LoopBack Test Mode.
 *
 *  Set the LoopBack Test Mode and Check if message send receives properly in FIFO.
 *
 * @param[in]  CAN Peripheral pointer
 *
 * @return status
 *
 */
uint8_t test_can_loopback_mode(can_regdef_t* p_can)
{
	uint32_t i = 0;
	uint8_t flag_received = 0;
	uint8_t flag_id_error = 0;
	uint8_t flag_length_error = 0;
	uint8_t flag_data_byte_error = 0;

	test_can_init_loopback_mode(CAN1);

	hal_can_transmit(CAN1, "PRASHANT",8,DATA_FRAME,STANDARD_FRAME,0x27);

	for(i = 0; i <= 100000;i++)
	{
		if (hal_can_receive(CAN1))
		{
			flag_received = 1;
			break;
		}
	}

	if(!flag_received)
	{
		uart_send_string("FAIL: No frame received in loopback mode \r\n");
		return -1;
	}

	if (can_rx.id != can_tx.id)
	{
		flag_id_error = 1;
		uart_send_string("FAIL: ID Mismatched \r\n");
		return -1;
	}

	if (can_rx.length != can_tx.length)
	{
		flag_length_error = 1;
		uart_send_string("FAIL: DLC Mismatched \r\n");
		return -1;
	}

	for (uint8_t i = 0; i < can_tx.length;i++)
	{
		if (can_tx.data.data_8[i] != can_rx.data_8[i])
		{
			flag_data_byte_error = 1;
			uart_send_string("FAIL: Data Byte Mismatched \r\n");
			return -1;
		}
	}

	if (flag_id_error == 0 && flag_length_error == 0 && flag_data_byte_error == 0)
	{
		uart_send_string("PASS: CAN Loopback Mode Test Passed! \r\n ");
		return 0;
	}


}
