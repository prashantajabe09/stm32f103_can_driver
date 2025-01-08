/*
 * bmp280.h
 *
 *  Created on: 19-Dec-2024
 *      Author: Prashant Ajabe
 */

#ifndef BMP280_H_
#define BMP280_H_

#include "spi.h"


/*
 * Max SPI speed @10Mhz
 * CPOL:CPHA = 00 and 11
 *
 *
 *
 */
uint8_t read_id();
void write_data_polling(uint8_t data);


#endif /* BMP280_H_ */
