/*
 * eeprom.h
 *
 *  Created on: 08-Dec-2024
 *      Author: Prashant Ajabe
 */

#ifndef EEPROM_H_
#define EEPROM_H_


#include "stm32f103x.h"
#include "I2C.h"


/*
 * eeprom word addresses
 */
#define FIRST_ADDRESS 	0x00
#define SECOND_ADDRESS	0x01
#define RANDOM_ADDRESS  0xAA


void eeprom_byte_write_polling(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint8_t byte);
void eeprom_byte_write_interrupt(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint8_t byte);
void eeprom_data_write_polling(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint32_t data);
void eeprom_data_write_interrupt(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint32_t data);
void eeprom_data_write_dma(i2cx_regdef_t *i2c_address, uint16_t data_word_address, uint32_t data);
uint8_t eeprom_byte_read_polling(i2cx_regdef_t *p_i2cx, uint16_t device_data_address);
void eeprom_byte_read_interrupt(i2cx_regdef_t *p_i2cx, uint16_t device_data_address);
uint32_t eeprom_data_read_polling(i2cx_regdef_t *i2c_address, uint16_t device_data_address);
void eeprom_data_read_interrupt(i2cx_regdef_t *i2c_address, uint16_t device_data_address);
void eeprom_data_read_dma(i2cx_regdef_t *i2c_address, uint16_t device_data_address);

#endif /* EEPROM_H_ */
