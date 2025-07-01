/*
 * can_test.h
 *
 *  Created on: Jun 25, 2025
 *      Author: PRASHANT AJABE
 */

#ifndef CAN_TEST_H_
#define CAN_TEST_H_

#include "stm32f103x.h"
//#include "CAN.h"

#define ASSERT_TEST(x) if (!(x)) error_handler(__FILE__, __LINE__)

void error_handler(const char *file, int line);

#define BTR_TEST_PASS		1
#define BTR_TEST_FAILED		0

void error_handler(const char *file, int line);
uint8_t test_can_btr(can_regdef_t* p_can,uint32_t baudrate,uint32_t expected_btr);
uint8_t test_can_filter_32_bit_id_list_mode(filter_index_e filter_index,uint32_t id, uint32_t mask, \
						uint8_t filter_assign_to, uint8_t ide, uint8_t rtr);
uint8_t test_can_filter_16_bit_id_list_mode(filter_index_e filter_index,uint16_t* id, \
		uint8_t filter_assign_to, uint8_t rtr);
uint8_t test_can_filter_32_bit_id_mask_mode(filter_index_e filter_index,uint32_t id, uint32_t mask, \
		uint8_t filter_assign_to, uint8_t ide, uint8_t rtr);
uint8_t test_can_filter_16_bit_id_mask_mode(filter_index_e filter_index,uint16_t* id, uint32_t* mask, \
		uint8_t filter_assign_to, uint8_t rtr);
void test_can_init_loopback_mode(can_regdef_t* p_can);
uint8_t test_can_loopback_mode();
#endif /* CAN_TEST_H_ */
