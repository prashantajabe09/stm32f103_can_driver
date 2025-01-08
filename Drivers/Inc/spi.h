/*
 * spi.h
 *
 *  Created on: 16-Dec-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

/* SPI1 PINS
 * PA4 --> SPI1_NSS
 * PA5 --> SPI1_SCK
 * PA6 --> SPI1_MISO
 * PA7 --> SPI1_MOSI
 *
 * SPI2 PINS
 * PB12 --> SPI2_NSS
 * PB13 --> SPI2_SCK
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 */

/*
 * SPI1 --> APB2
 * SPI2 and SPI3 --> APB1
 */

#include "stm32f103x.h"

/*
 * Bi-Directional data mode enable/disable
 */
#define BIDIMODE_DI 		0
#define BIDIMODE_EN			1

/*
 * Output enable in bidirectional mode
 */
#define BIDIOE_DI			0
#define BIDIOE_EN			1

/*
 * Data Frame Format
 */
#define EIGHT_BIT_DATA_FORMAT 		0
#define SIXTEEN_BIT_DATA_FORMAT		1


/*
 *	 receive only bit
 *	 Use in combination with BIDIMODE = 0 (2 line unidirectional mode)
 */
#define FULL_DUPLEX 	0
#define RECEIVE_ONLY	1

/*
 * Software slave management.
 * Used with SSI bit.
 */
#define SOFTWARE_SLAVE_MANAGEMENT_DI	0
#define SOFTWARE_SLAVE_MANAGEMENT_EN	1

/*
 * Internal Slave Select.
 * SSI Have effect only when SSM is set. This value is forced on NSS pin and IO value of the pin is ignored.
 */
#define SSI_LOW		0
#define SSI_HIGH	1

/*
 * LSBFIREST bit.
 */
#define MSB_TRANSMITTED_FIRST	0
#define LSB_TRANSMITTED_FIRST   1

/*
 * SPI EN/DI
 */
#define SPI_DI 		0
#define SPI_EN 		1

/*
 * Master selection
 */
#define SLAVE_CONFIGURATION  		0
#define MASTER_CONFIGURATION		1


/*
 * CPOL
 */
#define CK_TO_ZERO_WHEN_IDLE		0
#define CK_TO_ONE_WHEN_IDLE			1

/*
 * CPHA.
 * Edge selection (1st or 2nd) for data capture.
 */
#define FIRST_CLK_TRANSITION_FOR_DATA_CAPTURE	0
#define SECOND_CLK_TRANSITION_FOR_DATA_CAPTURE	1


/*
 * Baudrate Control
 *
 */
typedef enum{
	FPCLK_DIV_2,
	FPCLK_DIV_4,
	FPCLK_DIV_8,
	FPCLK_DIV_16,
	FPCLK_DIV_32,
	FPCLK_DIV_64,
	FPCLK_DIV_128,
	FPCLK_DIV_256,
}baudrate_e;


/*********** CR2 MACROS ***************/

/*
 * RX DMA EN/DI
 */
#define RX_DMA_DI  0
#define RX_DMA_EN  1

/*
 * TX DMA EN/DI
 */
#define TX_DMA_DI  0
#define TX_DMA_EN  1

/*
 * Error Interrupt EN/DI
 */
#define ERROR_INTERRUPT_DI			0
#define ERROR_INTERRUPT_EN			1

/*
 * Rx Buffer Not empty  EN/DI
 */
#define RXNE_INTERRUPT_DI			0
#define RXNE_INTERRUPT_EN			1

/*
 * Tx Buffer empty  EN/DI
 */
#define TXE_INTERRUPT_DI			0
#define TXE_INTERRUPT_EN			1

/*
 * Status register flags
 */

/*
 * SPIx index
 */
#define SPI_1		 0
#define SPI_2 		 1
#define SPI_3 		 2


/*
 * CR1 Bit Position Macros
 */

#define BIDIMODE_EN_DI_BIT_POSITION_CR1 				15
#define BIDIOE_EN_DI_BIT_POSITION_CR1				    14
#define DATA_FRAME_FORMAT_BIT_POSITION_CR1 				11
#define RXONLY_BIT_POSITION_CR1 						10
#define SW_SALVE_MGMNT_BIT_POSITION_CR1					 9
#define INTERNAL_SLAVE_SELECT_BIT_POSITION_CR1  		 8
#define LSB_FIRST_BIT_POSITION_CR1  					 7
#define SPI_EN_DI_BIT_POSTION_CR1					     6
#define BAUD_RATE_CONTROL_BIT_POSITION_CR1 				 3
#define MASTER_SELECTION_BIT_POSITION_CR1			     2
#define CPOL_BIT_POSITION_CR1 						     1
#define CPHA_BIT_POSITION_CR1							 0

/*
 * CR2 Bit Position Macros
 */
#define RX_DMA_EN_DI_BIT_POSITION					 0
#define TX_DMA_EN_DI_BIT_POSITION					 1
#define ERR_INTERRUPT_EN_DI_BIT_POSITION			 5
#define RXNE_INTERRUPT_EN_DI_BIT_POSITION 			 6
#define TX_INTERRUPT_EN_DI_BIT_POSITION				 7



/*
 * ERROR Interrupt En or Di
 */
#define ERROR_INTERRUPT_DI 			0
#define ERROR_INTERRUPT_EN 			1

/*
 * TXE Interrupt En or Di
 */
#define TXE_INTERRUPT_DI 			0
#define TXE_INTERRUPT_EN 			1

/*
 * RXNE Interrupt En or Di
 */
#define RXNE_INTERRUPT_DI 			0
#define RXNE_INTERRUPT_EN 			1

/*
 * Event Flag
 */
#define FLAG_RXNE				(1 << 0)
#define FLAG_TXE				(1 << 1)
#define FLAG_CHSIDE				(1 << 2)
#define FLAG_UDR				(1 << 3)
#define FLAG_CRC_ERR			(1 << 4)
#define FLAG_MODF				(1 << 5)
#define FLAG_OVR				(1 << 6)
#define FLAG_BSY				(1 << 7)





typedef struct{
	uint16_t bi_di_mode:1; 		      // BIDIMODE
	uint16_t software_slave_mngmt:1;     // Software slave management or hardware slave management
	uint16_t error_interrupt_en_or_di:1; // Error interrupt EN or DI
	uint16_t rxne_interrupt_en_or_di:1;  // RXNE interrupt EN or DI
	uint16_t txe_interrupt_en_or_di:1;   // TXE interrupt EN or DI
	uint16_t tx_dma_en:1;				  // TX DMA EN OR DI
	uint16_t rx_dma_en:1;				  // RX DMA EN OR DI
	uint16_t baudrate:3;				  // Baud-rate
	uint16_t cpol:1;					  // CPOL
	uint16_t cpha:1;					  // CPHA
	uint16_t master_selection:1;	      // SLAVE OR MASTER
	uint16_t first_bit_out:1;			  // MSB or LSB first
	uint16_t frame_length:1;			  // 8 bit or sixteen bit
	uint16_t rx_only:1; 				  // Full duplex or receive only
}spi_config;



//spi_config spi_x[3];
void spi_init(spix_regdef_t *p_spix,uint8_t channel);
void nss_pin_toggle(spix_regdef_t *p_spix,uint8_t mode);
void data_write(spix_regdef_t *p_spix,uint32_t data);
void waite_for_sr_flag_to_set(spix_regdef_t *p_spix,uint8_t flag);
uint8_t get_sr_flag_status(spix_regdef_t *p_spix,uint8_t flag);
void spi_en_di(spix_regdef_t *p_spix, uint8_t en_or_di);
void ssi_pin_select(spix_regdef_t *p_spix,uint8_t value);
uint8_t data_read(spix_regdef_t *p_spix);
void spi_1_slave_select();

#endif /* INC_SPI_H_ */
