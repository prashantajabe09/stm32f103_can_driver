/*
 * I2C.h
 *
 *  Created on: 23-Sep-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_


#include "stm32f103x.h"


extern uint8_t eeprom_buffer[64];
extern int8_t transmit_len;
extern uint8_t rx_i2c_buffer[3];
extern int8_t receive_len;
extern uint8_t flag_send_write_address;
extern uint8_t flag_interrupt_based_read;
extern uint8_t flag_temp,flag_read_multiple_byte;
extern uint8_t flag_dma_based_read;
/*
 * EEPROM ADDRESS
 *
 */
#define FIRST_ADDRESS 	0x00
#define SECOND_ADDRESS	0x01
#define RANDOM_ADDRESS  0xAA




/*
 * This Driver is for I2C master Transmitter and Receiver with polling, interrupt and DMA Method only.
 */

#include "stm32f103x.h"
/*
 * APB1 Bus
 * PB10 --> I2C2_SCL
 * PB11 --> I2C2_SDA
 */

typedef enum{
	I2C1_CHANNEL,
	I2C2_CHANNEL,
}i2c_channel_e;

typedef enum{
	SM_MODE,
	FM_MODE
}master_mode_e;

#define DMA_DI 0
#define DMA_EN 1

#define IT_BUF_DI 0
#define IT_BUF_EN 1

#define IT_EVT_DI 0
#define IT_EVT_EN 1

#define IT_ERR_DI 0
#define IT_ERR_EN 1

#define FM_DUTY_CYCLE_2 		0
#define FM_DUTY_CYCLE_16_9 		1


#define WRITE					0
#define READ					1

#define MASTER_MODE_SELECTION_BIT_POSITION_CCR	 	15
#define DMA_EN_BI_POSITION_CR2 						11
#define IT_BUF_EN_BIT_POSTITION_CR2					10
#define IT_EVT_EN_BIT_POSTITION_CR2					 9
#define IT_ERR_EN_BIT_POSTITION_CR2					 8
#define FM_DUTY_CYCLE_BIT_POSITION_CRR				14
#define START_CONDITION_BIT_POSITION_CR1		 	 8
#define ACK_BIT_POSITION_CR1						10
#define SB_FLAG_BIT_POSITION_SR1					 0
#define ADDR_FLAG_POSITION_SR1						 1
#define BTF_FLAG_POSITION_SR1						 2
#define TXE_FLAG_POSITION_SR1 						 7
#define RXNE_FLAG_POSITION_SR1						 6
#define PERIPHERAL_EN_BIT_POSITION_CR1				 0
#define TRANSMITTER_RECEIVER_BIT_POSTION_SR2	     2
#define I2CX_SOFTWARE_RESET_BIT_POSITION_CR1		 15

#define FLAG_MSL_SR2	(1 << 0)
#define FLAG_BUSY_SR2   (1 << 1)
#define FLAG_TRA_SR2	(1 << 2)
#define FLAG_SB_SR1		(1 << SB_FLAG_BIT_POSITION_SR1)
#define FLAG_ADDR_SR1   (1 << ADDR_FLAG_POSITION_SR1)
#define FLAG_BTF_SR1	(1 << BTF_FLAG_POSITION_SR1)
#define FLAG_TXE_SR1	(1 << TXE_FLAG_POSITION_SR1)
#define FLAG_RXNE_SR1	(1 << RXNE_FLAG_POSITION_SR1)
#define FLAG_TX_OR_RX	(1 << TRANSMITTER_RECEIVER_BIT_POSTION_SR2)


#define EEPROM_DEVICE_ADDRESS 						0xA0


typedef enum{
	PERIPHERAL_CLOCK_FREQ_2MHZ = 2,
	PERIPHERAL_CLOCK_FREQ_3MHZ,
	PERIPHERAL_CLOCK_FREQ_4MHZ,
	PERIPHERAL_CLOCK_FREQ_5MHZ,
	PERIPHERAL_CLOCK_FREQ_6MHZ,
	PERIPHERAL_CLOCK_FREQ_7MHZ,
	PERIPHERAL_CLOCK_FREQ_8MHZ,
	PERIPHERAL_CLOCK_FREQ_9MHZ,
	PERIPHERAL_CLOCK_FREQ_10MHZ,
	PERIPHERAL_CLOCK_FREQ_11MHZ,
	PERIPHERAL_CLOCK_FREQ_12MHZ,
	PERIPHERAL_CLOCK_FREQ_13MHZ,
	PERIPHERAL_CLOCK_FREQ_14MHZ,
	PERIPHERAL_CLOCK_FREQ_15MHZ,
	PERIPHERAL_CLOCK_FREQ_16MHZ,
	PERIPHERAL_CLOCK_FREQ_17MHZ,
	PERIPHERAL_CLOCK_FREQ_18MHZ,
	PERIPHERAL_CLOCK_FREQ_19MHZ,
	PERIPHERAL_CLOCK_FREQ_20MHZ,
	PERIPHERAL_CLOCK_FREQ_21MHZ,
	PERIPHERAL_CLOCK_FREQ_22MHZ,
	PERIPHERAL_CLOCK_FREQ_23MHZ,
	PERIPHERAL_CLOCK_FREQ_24MHZ,
	PERIPHERAL_CLOCK_FREQ_25MHZ,
	PERIPHERAL_CLOCK_FREQ_26MHZ,
	PERIPHERAL_CLOCK_FREQ_27MHZ,
	PERIPHERAL_CLOCK_FREQ_28MHZ,
	PERIPHERAL_CLOCK_FREQ_29MHZ,
	PERIPHERAL_CLOCK_FREQ_30MHZ,
	PERIPHERAL_CLOCK_FREQ_31MHZ,
	PERIPHERAL_CLOCK_FREQ_32MHZ,
	PERIPHERAL_CLOCK_FREQ_33MHZ,
	PERIPHERAL_CLOCK_FREQ_34MHZ,
	PERIPHERAL_CLOCK_FREQ_35MHZ,
	PERIPHERAL_CLOCK_FREQ_36MHZ,
	PERIPHERAL_CLOCK_FREQ_37MHZ,
	PERIPHERAL_CLOCK_FREQ_38MHZ,
	PERIPHERAL_CLOCK_FREQ_39MHZ,
	PERIPHERAL_CLOCK_FREQ_40MHZ,
	PERIPHERAL_CLOCK_FREQ_41MHZ,
	PERIPHERAL_CLOCK_FREQ_42MHZ,
	PERIPHERAL_CLOCK_FREQ_43MHZ,
	PERIPHERAL_CLOCK_FREQ_44MHZ,
	PERIPHERAL_CLOCK_FREQ_45MHZ,
	PERIPHERAL_CLOCK_FREQ_46MHZ,
	PERIPHERAL_CLOCK_FREQ_47MHZ,
	PERIPHERAL_CLOCK_FREQ_48MHZ,
	PERIPHERAL_CLOCK_FREQ_49MHZ,
	PERIPHERAL_CLOCK_FREQ_50MHZ,
}peripheral_clock_e;

typedef struct{
	uint8_t master_mode;			// FM or SM mode
	uint8_t dma_en_di; 				// DMA EN or DI
	uint8_t it_buf_en; 				// buffer interrupt en
	uint8_t it_evt_en; 				// event interrupt en
	uint8_t it_err_en; 				// error interrupt EN
	uint8_t peripheral_clock_freq;	//peripheral clock Frequency
	uint32_t ccr; 					// CCR value. Must be configured when I2C is disbaled.
	uint8_t fm_duty_cycle; 			// FM duty cycle
	uint32_t trise;					// Trise value
}i2c_channel_config;

void i2cx_init(i2cx_regdef_t *p_i2cx, uint8_t channel);
void write_7_bit_device_address(i2cx_regdef_t *p_i2cx, uint8_t address,uint8_t dir);
void i2c_byte_write_polling(i2cx_regdef_t *p_i2cx,uint16_t data_word_address, uint8_t byte);
void write_data(i2cx_regdef_t *p_i2cx,uint8_t *buffer);
void i2c_ack_en(i2cx_regdef_t *p_i2cx);
void i2c_ack_di(i2cx_regdef_t *p_i2cx);
void clear_addr_bit(i2cx_regdef_t *p_i2cx);
void send_start(i2cx_regdef_t *p_i2cx);
void send_stop(i2cx_regdef_t *p_i2cx);
void i2cx_en(i2cx_regdef_t *p_i2cx);
void i2cx_di(i2cx_regdef_t *p_i2cx);
void send_repeated_start(i2cx_regdef_t *p_i2cx);
uint8_t i2cx_read_data_register(i2cx_regdef_t *p_i2cx);
uint8_t get_flag_status_sr1(i2cx_regdef_t *p_i2cx,uint8_t flag);
uint8_t get_flag_status_sr2(i2cx_regdef_t *p_i2cx,uint8_t flag);
void single_byte_receive_operation(i2cx_regdef_t *p_i2cx);
void waite_for_sr1_flag_to_set(i2cx_regdef_t *p_i2cx,uint8_t flag);
void i2c2_buffer_interrupt_handling(void);
void i2cx_reset(i2cx_regdef_t *p_i2cx);
#endif /* INC_I2C_H_ */
