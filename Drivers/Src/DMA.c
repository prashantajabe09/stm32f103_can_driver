/*
 * DMA.c
 *
 *  Created on: 29-Nov-2024
 *      Author: Prashant Ajabe
 */

#include "DMA.h"

dma1_channel_config dma1_channel_configuration[7] = {
		[CHANNEL_4 - 1] = {(uint32_t)&eeprom_buffer[0],  (uint32_t)&(I2C2->DR),4,LOW,READ_FROM_MEMORY,CIRCULAR_MODE_DI,INCREMENT_MODE_DI,INCREMENT_MODE_EN,MS_8_BITS,TC_INTERRUPT_EN,DMA_CHANNEL_4_GLOBAL_IRQn,ENABLE},
		[CHANNEL_5 - 1] = {(uint32_t)&rx_i2c_buffer[0],  (uint32_t)&(I2C2->DR),4,LOW,READ_FROM_MEMORY,CIRCULAR_MODE_DI,INCREMENT_MODE_DI,INCREMENT_MODE_EN,MS_8_BITS,TC_INTERRUPT_EN,DMA_CHANNEL_5_GLOBAL_IRQn,ENABLE},
		[CHANNEL_6 - 1] = {(uint32_t)&tx_buffer_data[0], (uint32_t)&(USART2->DR),8,LOW,READ_FROM_PERIPHERAL,CIRCULAR_MODE_DI,INCREMENT_MODE_EN,INCREMENT_MODE_DI,MS_8_BITS,TC_INTERRUPT_EN,DMA_CHANNEL_6_GLOBAL_IRQn,ENABLE},
		[CHANNEL_7 - 1] = {(uint32_t)&tx_buffer_data[0], (uint32_t)&(USART2->DR),0,LOW,READ_FROM_PERIPHERAL,CIRCULAR_MODE_DI,INCREMENT_MODE_DI,INCREMENT_MODE_EN,MS_8_BITS,TC_INTERRUPT_EN,DMA_CHANNEL_7_GLOBAL_IRQn,DISABLE}
	};


void dmax_channel_init(dma1_regdef_t *p_dma_x,uint8_t channel)
{
	p_dma_x->dma_channel[channel - 1].CMAR = dma1_channel_configuration[channel - 1].source_address;

	p_dma_x->dma_channel[channel - 1].CPAR = dma1_channel_configuration[channel - 1].destination_address;

	p_dma_x->dma_channel[channel - 1].CNDTR = dma1_channel_configuration[channel - 1].data_count;

	if (dma1_channel_configuration[channel - 1].direction){
		p_dma_x->dma_channel[channel - 1].CCR |= DMA_DIRECTION_BIT_POSTION;
	}
	else
	{
		p_dma_x->dma_channel[channel - 1].CCR &= ~DMA_DIRECTION_BIT_POSTION;
	}

	p_dma_x->dma_channel[channel - 1].CCR &= ~(DMA_PRIORITY_BIT_POS_MASK);
	p_dma_x->dma_channel[channel - 1].CCR |= (dma1_channel_configuration[channel - 1].priority_level << DMA_PRIORITY_BIT_POSITON);

	if (dma1_channel_configuration[channel - 1].circular_mode){
		p_dma_x->dma_channel[channel - 1].CCR |= DMA_CIRCULAR_MODE_BTI_POSTION;
	}
	else
	{
		p_dma_x->dma_channel[channel - 1].CCR &= ~DMA_CIRCULAR_MODE_BTI_POSTION;
	}

	// TO DO: Data size default to 8 bit and increment mode default. write fucntion to do that.
	// TO DO: Write the function to set appropirate interrupt as currently just setting the TC interrupt
	p_dma_x->dma_channel[channel - 1].CCR |= (1 << 7);
	p_dma_x->dma_channel[channel - 1].CCR |= dma1_channel_configuration[channel - 1].interrupt_type;
	if (dma1_channel_configuration[channel - 1].interrupt_type != 0)
		nvic_en_irq(dma1_channel_configuration[channel - 1].dma_channel_irqn);

	if (dma1_channel_configuration[channel - 1].en_di)
	{

	}
		//p_dma_x->dma_channel[channel - 1].CCR |= (1 << 0);
}


void dma_counter_set(dma1_regdef_t *p_dma_x,uint8_t channel, uint16_t data_len)
{
	p_dma_x->dma_channel[channel - 1].CNDTR = data_len;
}


void dma_channel_en(dma1_regdef_t *p_dma_x,uint8_t channel, uint16_t en_di)
{
	if (en_di)
		p_dma_x->dma_channel[channel - 1].CCR |= (1 << 0);
	else
		p_dma_x->dma_channel[channel - 1].CCR &= ~(1 << 0);
}

void dma1_channel6_interrupt_handling(dma1_regdef_t *p_dma_x,usartx_regdef_t *p_usartx)
{
	if ( (p_dma_x->ISR >> 20) & 0x01)
	{
		p_dma_x->IFCR |= (1 << 20);
	}
}

void dma1_channel7_interrupt_handling(dma1_regdef_t *p_dma_x,usartx_regdef_t *p_usartx)
{
	if ( (p_dma_x->ISR >> 24) & 0x01)
	{
		p_dma_x->IFCR |= (1 << 24);

	}
}

void dma1_channel4_interrupt_handling(dma1_regdef_t *p_dma_x,i2cx_regdef_t *p_i2cx)
{

	if ( (p_dma_x->ISR >> 12) & 0x01)
	{
		p_dma_x->IFCR |= (1 << 12);
		dma_channel_en(DMA1,CHANNEL_4,DI);
		if (!flag_dma_based_read)
		{
			while(!get_flag_status_sr1(p_i2cx,FLAG_BTF_SR1));//waite for BTF to set.
			send_stop(p_i2cx); // send stop
		}
		else if(flag_dma_based_read)
		{
			while(!get_flag_status_sr1(p_i2cx,FLAG_BTF_SR1));//waite for BTF to set.
			send_repeated_start(I2C2);
		}
	}

}

void dma1_channel5_interrupt_handling(dma1_regdef_t *p_dma_x,i2cx_regdef_t *p_i2cx)
{

	if ( (p_dma_x->ISR >> 16) & 0x01)
	{
		p_dma_x->IFCR |= (1 << 16);						 //clear the interrupt
		p_i2cx->CR2 &= ~(1 << DMA_EN_BI_POSITION_CR2);	 //Disable the DMA
		//while(!get_flag_status_sr1(p_i2cx,FLAG_BTF_SR1));//waite for BTF to set.
		send_stop(p_i2cx); // send stop
	}

}

void DMA1_Channel4_IRQHandler(void)
{
	dma1_channel4_interrupt_handling(DMA1,I2C2);
}

void DMA1_Channel5_IRQHandler(void)
{
	dma1_channel5_interrupt_handling(DMA1,I2C2);
}

void DMA1_Channel6_IRQHandler(void)
{
	dma1_channel6_interrupt_handling(DMA1,USART2);
}

void DMA1_Channel7_IRQHandler(void)
{
	dma1_channel7_interrupt_handling(DMA1,USART2);
}

