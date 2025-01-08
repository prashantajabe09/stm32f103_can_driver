/*
 * bmp280.c
 *
 *  Created on: 19-Dec-2024
 *      Author: Prashant Ajabe
 */

#include "bmp280.h"


uint8_t read_id()
{
	uint16_t data_hold = 0;
	//spi_1_slave_select(); // Start Communication
	GPIOA->ODR &= ~(1 << 4);
	waite_for_sr_flag_to_set(SPI1,FLAG_TXE);
	data_write(SPI1,0xFA); // 0xD0 is the ID adddress. But we want to read the data hence MSB to 1 for read. for write access it would be 0x50
	waite_for_sr_flag_to_set(SPI1,FLAG_RXNE);
	data_hold = data_read(SPI1) ;
	waite_for_sr_flag_to_set(SPI1,FLAG_TXE);
	//data_write(SPI1,0xFF);
	//waite_for_sr_flag_to_set(SPI1,FLAG_RXNE);
	//data_hold = data_read(SPI1);
	//GPIOA->BSRR |= (1 << 4);
	GPIOA->ODR |= (1 << 4);
	return data_hold;
}

void write_data_polling(uint8_t data)
{

}
