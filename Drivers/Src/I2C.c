/*
 * I2C.c
 *
 *  Created on: 23-Sep-2024
 *      Author: Prashant Ajabe
 */


#include "I2C.h"

uint8_t eeprom_buffer[64];
int8_t transmit_len;
uint8_t rx_i2c_buffer[3];
int8_t receive_len;
uint8_t flag_send_write_address;
uint8_t flag_interrupt_based_read;
uint8_t flag_temp,flag_read_multiple_byte;
uint8_t flag_dma_based_read;

/*
 * APB1 Bus
 * PB10 --> I2C2_SCL
 * PB11 --> I2C2_SDA
 */

uint8_t index_count = 0;
uint8_t temp_transmit_len = 0;



i2c_channel_config i2c_channel[2] = {
		[I2C1_CHANNEL] = {SM_MODE,DMA_DI,IT_BUF_DI,IT_EVT_DI,IT_ERR_DI,PERIPHERAL_CLOCK_FREQ_8MHZ,0x28,FM_DUTY_CYCLE_2,0x3},
		[I2C2_CHANNEL] = {SM_MODE,DMA_EN,IT_BUF_DI,IT_EVT_EN,IT_ERR_DI,PERIPHERAL_CLOCK_FREQ_8MHZ,0x28,FM_DUTY_CYCLE_2,0x3},
};

#define I2C_SR2_BUSY  (1 << 1)
#define I2C_SR1_SB 	  (1 << 0)
#define I2C_SR1_ADDR  (1 << 1)
#define I2C_CR1_ACK	  (1 << 10)
#define I2C_CR1_STP   (1 << 9)
#define I2C_SR1_TXE	  (1 << 7)
#define I2C_SR1_RXNE  (1 << 6)

void i2cx_init(i2cx_regdef_t *p_i2cx, uint8_t channel)
{
	i2cx_reset(p_i2cx);
	// Master mode selection
	if (i2c_channel[channel].master_mode)
		p_i2cx->CCR |= i2c_channel[channel].master_mode << MASTER_MODE_SELECTION_BIT_POSITION_CCR;
	else
		p_i2cx->CCR &= ~(1 << MASTER_MODE_SELECTION_BIT_POSITION_CCR);

	// DMA Mode selection
	if (i2c_channel[channel].dma_en_di)
		p_i2cx->CR2 |= i2c_channel[channel].dma_en_di << DMA_EN_BI_POSITION_CR2;
	else
		p_i2cx->CR2 &= ~(1 << DMA_EN_BI_POSITION_CR2);

	//buffer interrupt en or di
	if (i2c_channel[channel].it_buf_en)
		p_i2cx->CR2 |= i2c_channel[channel].it_buf_en << IT_BUF_EN_BIT_POSTITION_CR2;
	else
		p_i2cx->CR2 &= ~(1 << IT_BUF_EN_BIT_POSTITION_CR2);

	//Event interrupt en or di
	if (i2c_channel[channel].it_evt_en)
			p_i2cx->CR2 |= i2c_channel[channel].it_evt_en << IT_EVT_EN_BIT_POSTITION_CR2;
		else
			p_i2cx->CR2 &= ~(1 << IT_EVT_EN_BIT_POSTITION_CR2);

	//error interrupt en or di
	if (i2c_channel[channel].it_err_en)
			p_i2cx->CR2 |= i2c_channel[channel].it_err_en << IT_ERR_EN_BIT_POSTITION_CR2;
		else
			p_i2cx->CR2 &= ~(1 << IT_ERR_EN_BIT_POSTITION_CR2);

	//peripheral clock Frequency
	p_i2cx->CR2 |= i2c_channel[channel].peripheral_clock_freq;

	// CCR value. Must be configured when I2C is disbaled.
	p_i2cx->CCR |= i2c_channel[channel].ccr;

	// FM duty cycle
	if (i2c_channel[channel].fm_duty_cycle)
		p_i2cx->CCR |= (FM_DUTY_CYCLE_16_9 << FM_DUTY_CYCLE_BIT_POSITION_CRR);
	else
		p_i2cx->CCR &= ~(1 << FM_DUTY_CYCLE_BIT_POSITION_CRR);

	// Trise value
	p_i2cx->TRISE |= i2c_channel[channel].trise;

	// enable the start bit set interrupt. control bit --> ITEVFEN, event flag SB.
	i2cx_en(p_i2cx);
	i2c_ack_en(p_i2cx);
	nvic_en_irq(I2C2_EV_IRQn);
}





void send_start(i2cx_regdef_t *p_i2cx)
{
	if (get_flag_status_sr2(I2C2,FLAG_BUSY_SR2)) // Check if line busy, if yes then print message else write start bit and waite.
	{
		// Print message of the line busy
	}
	else
		I2C2->CR1 |= (1 << START_CONDITION_BIT_POSITION_CR1);

}

void write_7_bit_device_address(i2cx_regdef_t *p_i2cx, uint8_t address,uint8_t dir)
{
	p_i2cx->DR = (address | dir);
}

void write_data(i2cx_regdef_t *p_i2cx,uint8_t *buffer)
{
	p_i2cx->DR = *buffer;
}


void send_stop(i2cx_regdef_t *p_i2cx)
{
	I2C2->CR1 |= I2C_CR1_STP;
}

void i2c_byte_write_polling(i2cx_regdef_t *p_i2cx,uint16_t data_word_address, uint8_t byte)
{
	uint8_t temp;
	send_start(p_i2cx);

	while(!get_flag_status_sr1(p_i2cx,FLAG_SB_SR1));
	write_7_bit_device_address(p_i2cx,EEPROM_DEVICE_ADDRESS,WRITE);

	while(!get_flag_status_sr1(p_i2cx,FLAG_ADDR_SR1));
	clear_addr_bit(p_i2cx);

	temp = ( data_word_address >> 8) & 0x00FF;  					// extract upper byte of the data  word address
	write_data(p_i2cx,&temp); 										// write upper byt of the device data address
	while(!get_flag_status_sr1(p_i2cx,FLAG_TXE_SR1));
	temp = data_word_address & 0x00FF; 							// extract lower byte of the data word address
	write_data(p_i2cx,&temp);
	while(!get_flag_status_sr1(p_i2cx,FLAG_TXE_SR1));
	write_data(p_i2cx,&byte);
	while(!get_flag_status_sr1(p_i2cx,FLAG_BTF_SR1));
	send_stop(p_i2cx);
}

uint8_t get_flag_status_sr1(i2cx_regdef_t *p_i2cx, uint8_t flag)
{

	if (p_i2cx->SR1 & flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t get_flag_status_sr2(i2cx_regdef_t *p_i2cx, uint8_t flag)
{

	if (p_i2cx->SR2 & flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void i2c_ack_en(i2cx_regdef_t *p_i2cx)
{
	p_i2cx->CR1 |= (1 << ACK_BIT_POSITION_CR1);
}

void i2c_ack_di(i2cx_regdef_t *p_i2cx)
{
	p_i2cx->CR1 &= ~(1 << ACK_BIT_POSITION_CR1);
}

void clear_addr_bit(i2cx_regdef_t *p_i2cx)
{
	uint32_t temp;
	temp = p_i2cx->SR1;
	temp = p_i2cx->SR2;
}


uint8_t i2cx_read_data_register(i2cx_regdef_t *p_i2cx)
{
	return p_i2cx->DR;
}

void i2cx_en(i2cx_regdef_t *p_i2cx)
{
	p_i2cx->CR1 |= (1 << PERIPHERAL_EN_BIT_POSITION_CR1);
}

void i2cx_di(i2cx_regdef_t *p_i2cx)
{
	p_i2cx->CR1 &= ~(1 << PERIPHERAL_EN_BIT_POSITION_CR1);
}

void send_repeated_start(i2cx_regdef_t *p_i2cx)
{
	I2C2->CR1 |= (1 << START_CONDITION_BIT_POSITION_CR1);
	//while(!(I2C2->SR1 & I2C_SR1_SB)){;}
}



void single_byte_receive_operation(i2cx_regdef_t *p_i2cx)
{
	send_stop(p_i2cx);
}

void waite_for_sr1_flag_to_set(i2cx_regdef_t *p_i2cx,uint8_t flag)
{
	while(!get_flag_status_sr1(p_i2cx,flag));
}


void I2C2_EV_IRQHandler(void)
{
	uint32_t temp;
	if (I2C2->CR2 & (1 << 9))
	{
		if (get_flag_status_sr1(I2C2,FLAG_SB_SR1) == 1)
		{
			temp = I2C2->SR1;
			if (flag_send_write_address)
			{
				write_7_bit_device_address(I2C2,EEPROM_DEVICE_ADDRESS,WRITE);
			}
			else if(!flag_send_write_address)
			{
				write_7_bit_device_address(I2C2,EEPROM_DEVICE_ADDRESS,READ);
			}

		}

		if (get_flag_status_sr1(I2C2,FLAG_ADDR_SR1) == 1)
		{
			if (flag_send_write_address)
			{
				clear_addr_bit(I2C2);
				flag_send_write_address = 0;
				if(flag_dma_based_read)
				{
					dma_counter_set(DMA1,CHANNEL_4,2);
					dma_channel_en(DMA1,CHANNEL_4,1);
				}
			}
			else if (!flag_send_write_address)
			{
				clear_addr_bit(I2C2);
				if (!flag_read_multiple_byte)
				{
					i2c_ack_di(I2C2);
					send_stop(I2C2);
				}
			}
		}

		if (get_flag_status_sr1(I2C2,FLAG_BTF_SR1) == 1)
		{
			send_stop(I2C2);
		}
	}

	if (I2C2->CR2 & (1 << 10))
	{

		i2c2_buffer_interrupt_handling();
	}
}





void i2c2_buffer_interrupt_handling(void)
{
	static uint8_t i = 0;
	static uint8_t j = 0;
	if (get_flag_status_sr2(I2C2,FLAG_TX_OR_RX) == 1)
	{
		if (get_flag_status_sr1(I2C2,FLAG_TXE_SR1) == 1)
		{
			if (flag_temp)
			{
				flag_temp = 0;
				temp_transmit_len = transmit_len;
			}

			if (transmit_len-- > 0)
			{

				I2C2->DR = eeprom_buffer[i++];
			}
			else
			{
				if (!flag_interrupt_based_read)
				{
					//I2C2->CR2 &= ~(1 << IT_BUF_EN_BIT_POSTITION_CR2);
				}

				if (flag_interrupt_based_read)
				{
					flag_interrupt_based_read = 0;
					send_repeated_start(I2C2);
				}
			}
		}
		else
		{

		}
	}
	else if (get_flag_status_sr2(I2C2,FLAG_TX_OR_RX) == 0)
	{
		if (get_flag_status_sr1(I2C2,FLAG_RXNE_SR1) == 1)
		{
			if (!flag_read_multiple_byte)
			{
				if (receive_len-- > 0)
				{
					rx_i2c_buffer[j++] = I2C2->DR;
				}
				else
				{
					I2C2->CR2 &= ~(1 << IT_BUF_EN_BIT_POSTITION_CR2);
				}
			}
			else if(flag_read_multiple_byte)
			{
				if (receive_len > 2)
				{
					rx_i2c_buffer[j++] = I2C2->DR;
					receive_len -= 1;
				}
				else
				{
					if (receive_len == 2)
					{
						rx_i2c_buffer[j++] = I2C2->DR;
						i2c_ack_di(I2C2);
						send_stop(I2C2);
						receive_len -= 1;
					}
					else
					{
						rx_i2c_buffer[j++] = I2C2->DR;
						flag_read_multiple_byte = 0;
						//I2C2->CR2 &= ~(1 << IT_BUF_EN_BIT_POSTITION_CR2);
					}

				}
			}
		}
	}

}

void i2cx_reset(i2cx_regdef_t *p_i2cx)
{
	p_i2cx->CR1 |= (1 << I2CX_SOFTWARE_RESET_BIT_POSITION_CR1);
	while(p_i2cx->SR2 & (1<< 1)); // waite for busy flag to reset
	p_i2cx->CR1 &= ~(1 << I2CX_SOFTWARE_RESET_BIT_POSITION_CR1);
}
