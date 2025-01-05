/*
 * eeprom.c
 *
 *  Created on: 08-Dec-2024
 *      Author: Prashant Ajabe
 */
/*
 * The 5 ms cycle time applies regardless of whether you write one byte or a full page.
 */
#include "eeprom.h"
#include "I2C.h"
#include "DMA.h"

uint32_t count_e_1,count_e_2,count_e_3,count_e_4,count_e_5;

void eeprom_byte_write_polling(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint8_t byte)
{
	uint8_t temp;

	send_start(i2c_address);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_SB_SR1);

	write_7_bit_device_address(i2c_address,EEPROM_DEVICE_ADDRESS,WRITE);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_ADDR_SR1);
	clear_addr_bit(i2c_address);

	temp = ( data_word_address >> 8) & 0x00FF;
	write_data(i2c_address,&temp);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_TXE_SR1);

	temp = data_word_address & 0x00FF;
	write_data(i2c_address,&temp);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_TXE_SR1);

	write_data(i2c_address,&byte);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_BTF_SR1);

	send_stop(i2c_address);
}

uint8_t eeprom_byte_read_polling(i2cx_regdef_t *i2c_address, uint16_t device_data_address)
{
	uint8_t temp,data;

	send_start(i2c_address);                                             // Send start condition
	waite_for_sr1_flag_to_set(i2c_address,FLAG_SB_SR1);                //Waite for start bit to set

	write_7_bit_device_address(i2c_address,EEPROM_DEVICE_ADDRESS,WRITE); // Write device address
	waite_for_sr1_flag_to_set(i2c_address,FLAG_ADDR_SR1);
	clear_addr_bit(i2c_address); 										// Clear the addr bit

	temp = ( device_data_address >> 8) & 0x00FF;  					// extract upper byte of the data  word address
	write_data(i2c_address,&temp); 										// write upper byte of the device data address
	waite_for_sr1_flag_to_set(i2c_address,FLAG_TXE_SR1);

	temp = device_data_address & 0x00FF; 							// extract lower byte of the data word address
	write_data(i2c_address,&temp);										// write upper byte of the device data address
	waite_for_sr1_flag_to_set(i2c_address,FLAG_BTF_SR1);

	send_repeated_start(i2c_address);                                             // Send start condition
	waite_for_sr1_flag_to_set(i2c_address,FLAG_SB_SR1);				// function for repeated start

	write_7_bit_device_address(i2c_address,EEPROM_DEVICE_ADDRESS,READ); 	// Write device address with read bit
	waite_for_sr1_flag_to_set(i2c_address,FLAG_ADDR_SR1);
	clear_addr_bit(i2c_address);

	single_byte_receive_operation(i2c_address);

	waite_for_sr1_flag_to_set(i2c_address,FLAG_RXNE_SR1);
	data = i2cx_read_data_register(i2c_address); 						// fucntion to read byte from DR
	return data;
}

void eeprom_byte_read_interrupt(i2cx_regdef_t *p_i2cx, uint16_t device_data_address)
{
	count_e_1++;
	eeprom_buffer[0] = ( (device_data_address >> 8) & 0x00FF);
	eeprom_buffer[1] = device_data_address & 0x00FF;
	transmit_len = 2;
	receive_len = 1;
	flag_send_write_address = 1;
	flag_interrupt_based_read = 1;
	send_repeated_start(p_i2cx);

}

void eeprom_data_write_polling(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint32_t data)
{

	uint8_t temp,i;
	uint8_t data_array[4] = {0,0,0,0};
	data_array[0] = (data >> 0) & 0xFF;
	data_array[1] = (data >> 8) & 0xFF;
	data_array[2] = (data >> 16) & 0xFF;
	data_array[3] = (data >> 24) & 0xFF;

	send_start(i2c_address);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_SB_SR1);

	write_7_bit_device_address(i2c_address,EEPROM_DEVICE_ADDRESS,WRITE);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_ADDR_SR1);
	clear_addr_bit(i2c_address);

	temp = ( data_word_address >> 8) & 0x00FF;
	write_data(i2c_address,&temp);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_TXE_SR1);

	temp = data_word_address & 0x00FF;
	write_data(i2c_address,&temp);
	waite_for_sr1_flag_to_set(i2c_address,FLAG_TXE_SR1);
	for(i = 0;i < 4;i++)
	{
		waite_for_sr1_flag_to_set(i2c_address,FLAG_TXE_SR1);
		write_data(i2c_address,&data_array[i]);
	}
	waite_for_sr1_flag_to_set(i2c_address,FLAG_BTF_SR1);

	send_stop(i2c_address);
}

uint32_t eeprom_data_read_polling(i2cx_regdef_t *i2c_address, uint16_t device_data_address)
{
		uint8_t temp,read_len = 4;
		uint32_t data;
		send_start(i2c_address);                                             // Send start condition
		waite_for_sr1_flag_to_set(i2c_address,FLAG_SB_SR1);                //Waite for start bit to set

		write_7_bit_device_address(i2c_address,EEPROM_DEVICE_ADDRESS,WRITE); // Write device address
		waite_for_sr1_flag_to_set(i2c_address,FLAG_ADDR_SR1);
		clear_addr_bit(i2c_address); 										// Clear the addr bit

		temp = ( device_data_address >> 8) & 0x00FF;  					// extract upper byte of the data  word address
		write_data(i2c_address,&temp); 										// write upper byte of the device data address
		waite_for_sr1_flag_to_set(i2c_address,FLAG_TXE_SR1);

		temp = device_data_address & 0x00FF; 							// extract lower byte of the data word address
		write_data(i2c_address,&temp);										// write upper byte of the device data address
		waite_for_sr1_flag_to_set(i2c_address,FLAG_BTF_SR1);

		send_repeated_start(i2c_address);                                             // Send start condition
		waite_for_sr1_flag_to_set(i2c_address,FLAG_SB_SR1);				// function for repeated start

		write_7_bit_device_address(i2c_address,EEPROM_DEVICE_ADDRESS,READ); 	// Write device address with read bit
		waite_for_sr1_flag_to_set(i2c_address,FLAG_ADDR_SR1);
		clear_addr_bit(i2c_address);

		while(read_len-- != 3)
		{
			waite_for_sr1_flag_to_set(i2c_address,FLAG_RXNE_SR1);
			data = i2cx_read_data_register(i2c_address);
		}
		waite_for_sr1_flag_to_set(i2c_address,FLAG_BTF_SR1);
		i2c_ack_en(i2c_address);
		data |= (i2cx_read_data_register(i2c_address) << 8);
		send_stop(i2c_address);
		data |= (i2cx_read_data_register(i2c_address) << 16);
		waite_for_sr1_flag_to_set(i2c_address,FLAG_RXNE_SR1);
		data |= (i2cx_read_data_register(i2c_address) << 24);


		return data;
}

void eeprom_data_read_interrupt(i2cx_regdef_t *i2c_address, uint16_t device_data_address)
{
	eeprom_buffer[0] = ( (device_data_address >> 8) & 0x00FF);
	eeprom_buffer[1] = device_data_address & 0x00FF;
	transmit_len = 2;
	receive_len = 2;
	flag_send_write_address = 1;
	flag_interrupt_based_read = 1;
	flag_read_multiple_byte = 1;
	send_repeated_start(i2c_address);
}


void eeprom_data_read_dma(i2cx_regdef_t *i2c_address, uint16_t device_data_address)
{
	eeprom_buffer[0] = ( (device_data_address >> 8) & 0x00FF);
	eeprom_buffer[1] = device_data_address & 0x00FF;
	transmit_len = 2;
	receive_len = 4;
	dma_counter_set(DMA1,CHANNEL_5,4);
	dma_channel_en(DMA1,CHANNEL_5,1);
	flag_send_write_address = 1;
	flag_dma_based_read = 1;
	flag_read_multiple_byte = 1;
	i2c_address->CR2 |= (1 << 12);
	send_start(i2c_address);
}

void eeprom_byte_write_interrupt(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint8_t byte)
{
	eeprom_buffer[0] = ( (data_word_address >> 8) & 0x00FF);
	eeprom_buffer[1] = data_word_address & 0x00FF;
	eeprom_buffer[2] = byte;
	transmit_len = 3;
	flag_send_write_address = 1;
	flag_temp = 1;
	send_start(i2c_address);
}

void eeprom_write_interrupt(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint32_t data)
{
	eeprom_buffer[0] = ( (data_word_address >> 8) & 0x00FF);
	eeprom_buffer[1] = data_word_address & 0x00FF;
	eeprom_buffer[2] = (data >> 0)  & 0xFF;
	eeprom_buffer[3] = (data >> 8)  & 0xFF;
	eeprom_buffer[4] = (data >> 16) & 0xFF;
	eeprom_buffer[5] = (data >> 24) & 0xFF;
	transmit_len = 6;
	flag_send_write_address = 1;
	send_start(i2c_address);
}

void eeprom_data_write_dma(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint32_t data)
{
	eeprom_buffer[0] = ( (data_word_address >> 8) & 0x00FF);
	eeprom_buffer[1] = data_word_address & 0x00FF;
	eeprom_buffer[2] = (data >> 0)  & 0xFF;
	eeprom_buffer[3] = (data >> 8)  & 0xFF;
	eeprom_buffer[4] = (data >> 16) & 0xFF;
	eeprom_buffer[5] = (data >> 24) & 0xFF;
	dma_counter_set(DMA1,CHANNEL_4,6);
	dma_channel_en(DMA1,CHANNEL_4,1);
	flag_send_write_address = 1;
	send_start(i2c_address);
}
