/*
 * DMA.h
 *
 *  Created on: 29-Nov-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_

#include "stm32f103x.h"


typedef enum{
	READ_FROM_PERIPHERAL,
	READ_FROM_MEMORY,
}data_transfer_dir_e;

#define DMA_DIRECTION_BIT_POSTION	 		(1 << 4)
#define DMA_PRIORITY_BIT_POS_MASK	  		(3 << 12)
#define DMA_PRIORITY_BIT_POSITON			12
#define DMA_CIRCULAR_MODE_BTI_POSTION		(1 << 5)
#define DMA_DATA_SIZE_BIT_POS_MASK			(3 << 8)

/*
 * priority level Enum
 */
typedef enum{
	LOW,
	MEDIUM,
	HIGH,
	VERY_HIGH,
}channel_priority_e;

/*
 * circular mode Enum or macro
 */
typedef enum{
	CIRCULAR_MODE_DI,
	CIRCULAR_MODE_EN,
}ciruclar_mode_e;

/*
 * increment mode enum or macro
 */
typedef enum{
	INCREMENT_MODE_DI,
	INCREMENT_MODE_EN,
}increment_mode_e;

/*
 * data size enum
 */
typedef enum{
	MS_8_BITS,
	MS_16_BITS,
	MS_32_BITS,
	RESERVED,
}data_size_e;



/*
 * interrupt type enum or macro
 */

#define TC_INTERRUPT_EN (1 << 1)
#define HT_INTERRUPT_EN (1 << 2)
#define TE_INTERRUPT_EN (1 << 3)

#define ENABLE  1
#define DISABLE 0

typedef enum{
	CHANNEL_1 = 1,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4,
	CHANNEL_5,
	CHANNEL_6,
	CHANNEL_7,

}dma1_channel_e;

typedef struct
{
	//uint8_t channel;
	uint32_t source_address;// Source Address
	uint32_t destination_address;// Destination Address
	uint32_t data_count;// Total Number of the data
	uint32_t priority_level;// Channel priority
	uint8_t direction;
	uint8_t circular_mode;
	uint8_t peripheral_increment_mode;
	uint8_t memory_increment_mode;
	uint8_t data_size;
	uint8_t interrupt_type;
	irqn_type dma_channel_irqn;
	uint8_t en_di;
}dma1_channel_config;




extern dma1_channel_config dma1_channel_configuration[];

void dmax_channel_init(dma1_regdef_t *p_dma_x,uint8_t channel);
void dma_counter_set(dma1_regdef_t *p_dma_x,uint8_t channel, uint16_t data_len);
void dma_channel_en(dma1_regdef_t *p_dma_x,uint8_t channel, uint16_t en_di);
void dma1_channel4_interrupt_handling(dma1_regdef_t *p_dma_x,i2cx_regdef_t *p_i2cx);
void dma1_channel5_interrupt_handling(dma1_regdef_t *p_dma_x,i2cx_regdef_t *p_i2cx);
void dma1_channel6_interrupt_handling(dma1_regdef_t *p_dma_x,usartx_regdef_t *p_usartx);
void dma1_channel7_interrupt_handling(dma1_regdef_t *p_dma_x,usartx_regdef_t *p_usartx);

#endif /* INC_DMA_H_ */
