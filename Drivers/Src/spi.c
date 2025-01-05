/*
 * spi.c
 *
 *  Created on: 16-Dec-2024
 *      Author: Prashant Ajabe
 */
#include "spi.h"

spi_config spi_x[3] = {
		[SPI_1] = {BIDIMODE_DI,SOFTWARE_SLAVE_MANAGEMENT_EN,ERROR_INTERRUPT_DI,TXE_INTERRUPT_DI,RXNE_INTERRUPT_DI,TX_DMA_DI,RX_DMA_DI,FPCLK_DIV_2,CK_TO_ZERO_WHEN_IDLE,FIRST_CLK_TRANSITION_FOR_DATA_CAPTURE,MASTER_CONFIGURATION,MSB_TRANSMITTED_FIRST,EIGHT_BIT_DATA_FORMAT,FULL_DUPLEX},
		[SPI_2] = {BIDIMODE_DI,SOFTWARE_SLAVE_MANAGEMENT_EN,ERROR_INTERRUPT_DI,TXE_INTERRUPT_DI,RXNE_INTERRUPT_DI,TX_DMA_DI,RX_DMA_DI,FPCLK_DIV_2,CK_TO_ZERO_WHEN_IDLE,FIRST_CLK_TRANSITION_FOR_DATA_CAPTURE,MASTER_CONFIGURATION,MSB_TRANSMITTED_FIRST,EIGHT_BIT_DATA_FORMAT,FULL_DUPLEX},
		[SPI_3] = {BIDIMODE_DI,SOFTWARE_SLAVE_MANAGEMENT_EN,ERROR_INTERRUPT_DI,TXE_INTERRUPT_DI,RXNE_INTERRUPT_DI,TX_DMA_DI,RX_DMA_DI,FPCLK_DIV_2,CK_TO_ZERO_WHEN_IDLE,FIRST_CLK_TRANSITION_FOR_DATA_CAPTURE,MASTER_CONFIGURATION,MSB_TRANSMITTED_FIRST,EIGHT_BIT_DATA_FORMAT,FULL_DUPLEX},
};

void spi_init(spix_regdef_t *p_spix,uint8_t channel)
{
	// BI_Directional En or Di
	if (spi_x[channel].bi_di_mode)
		p_spix->CR1 |= 1 << BIDIMODE_EN_DI_BIT_POSITION;
	else
		p_spix->CR1 &= ~(1 << BIDIMODE_EN_DI_BIT_POSITION);


	// Software slave managment
	if (spi_x[channel].software_slave_mngmt)
		p_spix->CR1 |= 1 << SW_SALVE_MGMNT_BIT_POSITION;
	else
		p_spix->CR1 &= ~(1 << SW_SALVE_MGMNT_BIT_POSITION);

	// Error Interrupt EN DI
	if (spi_x[channel].error_interrupt_en_or_di)
		p_spix->CR2 |= 1 << ERR_INTERRUPT_EN_DI_BIT_POSITION;
	else
		p_spix->CR2 &= ~(1 << ERR_INTERRUPT_EN_DI_BIT_POSITION);

	// RXNE Interrupt EN DI
	if (spi_x[channel].rxne_interrupt_en_or_di)
		p_spix->CR2 |= 1 << RXNE_INTERRUPT_EN_DI_BIT_POSITION;
	else
		p_spix->CR2 &= ~(1 << RXNE_INTERRUPT_EN_DI_BIT_POSITION);

	// TX Interrupt EN DI
	if (spi_x[channel].txe_interrupt_en_or_di)
		p_spix->CR2 |= 1 << TX_INTERRUPT_EN_DI_BIT_POSITION;
	else
		p_spix->CR2 &= ~(1 << TX_INTERRUPT_EN_DI_BIT_POSITION);


	// TX DMA EN DI
	if (spi_x[channel].tx_dma_en)
		p_spix->CR2 |= 1 << TX_DMA_EN_DI_BIT_POSITION;
	else
		p_spix->CR2 &= ~(1 << TX_DMA_EN_DI_BIT_POSITION);

	// RXNE DMA EN DI
	if (spi_x[channel].rx_dma_en)
		p_spix->CR2 |= 1 << RX_DMA_EN_DI_BIT_POSITION;
	else
		p_spix->CR2 &= ~(1 << RX_DMA_EN_DI_BIT_POSITION);

	//Baudrate
	p_spix->CR1 &= ~(0x07 << BAUD_RATE_CONTROL_BIT_POSITION);
	p_spix->CR1 |= (spi_x[channel].baudrate << BAUD_RATE_CONTROL_BIT_POSITION);

	// CPOL
	if (spi_x[channel].cpol)
		p_spix->CR1 |= 1 << CPOL_BIT_POSITION;
	else
		p_spix->CR1 &= ~(1 << CPOL_BIT_POSITION);

	//CPHA
	if (spi_x[channel].cpha)
		p_spix->CR1 |= 1 << CPHA_BIT_POSITION;
	else
		p_spix->CR1 &= ~(1 << CPHA_BIT_POSITION);


	// Master select
	if (spi_x[channel].master_selection)
		p_spix->CR1 |= 1 << MASTER_SELECTION_BIT_POSITION;
	else
		p_spix->CR1 &= ~(1 << MASTER_SELECTION_BIT_POSITION);

	//First bit out
	if (spi_x[channel].first_bit_out)
		p_spix->CR1 |= 1 << LSB_FIRST_BIT_POSITION;
	else
		p_spix->CR1 &= ~(1 << LSB_FIRST_BIT_POSITION);

	//Frame Length
	if (spi_x[channel].frame_length)
		p_spix->CR1 |= 1 << DATA_FRAME_FORMAT_BIT_POSITION;
	else
		p_spix->CR1 &= ~(1 << DATA_FRAME_FORMAT_BIT_POSITION);

	//Rx_only
	if (spi_x[channel].rx_only)
		p_spix->CR1 |= 1 << RXONLY_BIT_POSITION;
	else
		p_spix->CR1 &= ~(1 << RXONLY_BIT_POSITION);

}

void nss_pin_toggle(spix_regdef_t *p_spix,uint8_t mode)
{

}

void data_write(spix_regdef_t *p_spix,uint32_t data)
{
	p_spix->DR = data;
}

void waite_for_sr_flag_to_set(spix_regdef_t *p_spix,uint8_t flag)
{
	while(!get_sr_flag_status(p_spix,flag));
}

void spi_en_di(spix_regdef_t *p_spix, uint8_t en_or_di)
{
	if (en_or_di)
	{
		p_spix->CR1 |= 1 << SPI_EN_DI_BIT_POSTION;
	}
	else
	{
		p_spix->CR1 &= ~(1 << SPI_EN_DI_BIT_POSTION);
	}
}

uint8_t get_sr_flag_status(spix_regdef_t *p_spix,uint8_t flag)
{
	if(p_spix->SR & flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
