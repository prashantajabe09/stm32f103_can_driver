/*
 * Timer.h
 *
 *  Created on: 21-May-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_
#include "stm32f103x.h"


/*
 * Mask
 */
#define MASK_FOR_CLEAR_2_BIT  0x03
#define MASK_FOR_CLEAR_3_BIT  0x07
#define MASK_FOR_CLEAR_4_BIT  0x0F

/*
 * capture compare enable register
 */
#define CAPTURE_COMPARE_1_OUTPUT_EN 						(1 << 0)
#define CAPTURE_COMPARE_1_OUTPUT_POLARITY     				(1 << 1)
#define CAPTURE_COMPARE_1_COMPLEMENTARY_OUTPUT_EN   		(1 << 2)
#define CAPTURE_COMPARE_1_COMPLEMENTARY_OUTPUT_POLARITY		(1 << 3)
#define CAPTURE_COMPARE_2_OUTPUT_EN							(1 << 4)
#define CAPTURE_COMPARE_2_OUTPUT_POLARITY     				(1 << 5)
#define CAPTURE_COMPARE_2_COMPLEMENTARY_OUTPUT_EN   		(1 << 6)
#define CAPTURE_COMPARE_2_COMPLEMENTARY_OUTPUT_POLARITY		(1 << 7)
#define CAPTURE_COMPARE_3_OUTPUT_EN							(1 << 8)
#define CAPTURE_COMPARE_3_OUTPUT_POLARITY     				(1 << 9)
#define CAPTURE_COMPARE_3_COMPLEMENTARY_OUTPUT_EN   		(1 << 10)
#define CAPTURE_COMPARE_3_COMPLEMENTARY_OUTPUT_POLARITY		(1 << 11)
#define CAPTURE_COMPARE_4_OUTPUT_EN							(1 << 12)
#define CAPTURE_COMPARE_4_OUTPUT_POLARITY     				(1 << 13)
#define CAPTURE_COMPARE_4_COMPLEMENTARY_OUTPUT_EN   		(1 << 14)
#define CAPTURE_COMPARE_4_COMPLEMENTARY_OUTPUT_POLARITY		(1 << 15)


/*
 * Timer status flag
 */
#define TIM_SR_UI_FLAG		(1 << 0)
#define TIM_SR_CC1I_FLAG	(1 << 1)
#define TIM_SR_CC2I_FLAG	(1 << 2)
#define TIM_SR_CC3I_FLAG    (1 << 3)
#define TIM_SR_CC4I_FLAG	(1 << 4)
#define TIM_SR_COMI_FLAG	(1 << 5)
#define TIM_SR_TI_FLAG      (1 << 6)
#define TIM_SR_BI_FLAG		(1 << 7)
#define TIM_SR_CC1O_FLAG	(1 << 9)
#define TIM_SR_CC2O_FLAG    (1 << 10)
#define TIM_SR_CC3O_FLAG    (1 << 11)
#define TIM_SR_CC4O_FLAG	(1 << 12)




/*
 * timer interrupt macros
 */
#define TIM_DIER_UI_EN		(1 << 0)
#define TIM_DIER_CC1I_EN    (1 << 1)
#define TIM_DIER_CC2I_EN    (1 << 2)
#define TIM_DIER_CC3I_EN    (1 << 3)
#define TIM_DIER_CC4I_EN    (1 << 4)
#define	TIM_DIER_UDE_EN		(1 << 8)
#define TIM_DIER_CC1DE_EN	(1 << 9)
#define TIM_DIER_CC2DE_EN   (1 << 10)
#define TIM_DIER_CC3DE_EN	(1 << 11)
#define TIM_DIER_CC4DE_EN   (1 << 12)


/*
 * compare output polarity
 */
#define CC_X_ACTIVE_HIGH	 0
#define CC_X_ACTIVE_LOW  	 1

/*
 * input capture polarity
 */
#define IC_X_RISING			0
#define IC_X_FALLING		1
/*
 * bit positions
 */
#define COUNTER_EN_BIT_POSTION_CR1		 		 0
#define CMS_BIT_POSTION_CR1 			 		 5
#define COUNTING_DIR_BIT_POSITION_CR1			 4

#define OC_1_EN_BIT_POSITION_CCER		 		 0
#define OC_1_POLARITY_BIT_POSITION_CCER  		 1

#define MAIN_OUTPUT_EN_BIT_POSITION_BDTR 		15

/*
 * Center Aligned Mode selection
 */
#define EDGE_ALIGNED_MODE 			0 //Counter counts up or down depending on dir bit
#define CENTER_ALIGNED_MODE_1		1 // counter counts up or down alternatively and OCxIF set only when the counter is counting down.
#define CENTER_ALIGNED_MODE_2		2 // counter counts up or down alternatively and OCxIF set only when the counter is counting up.
#define CENTER_ALIGNED_MODE_3		3 // counter counts up or down alternatively and OCxIF set only when the counter is counting up and down.


/*
 * Output compare mode
 */
#define FROZEN				0
#define ACTIVE_LEVEL		1
#define INACTIVE_LEVEL		2
#define TOGGLE 				3
#define FORCED_INACTIVE		4
#define FORCED_ACTIVE		5
#define PWM_MODE_1			6
#define PWM_MODE_2			7


/*
 * capture/compare selection (INPUT OR OUTPUT)
 */
#define CC_X_AS_OUTPUT              0
#define CC_X_AS_INPUT_IC_X_TO_TI_X    1
#define CC_X_AS_INPUT_IC_X_TO_TI_Y    2
#define CC_X_AS_INPUT_IC2_TO_TRC    3

/*
 * Counting Direction
 */
#define UP_COUNTING					0
#define DOWN_COUNTING				1

/*
 * Input capture x prescalar
 */
#define NO_PRESCALER 				0
#define CAPTURE_AT_EVERY_2_EVENTS	1
#define CAPTURE_AT_EVERY_4_EVENTS	2
#define CAPTURE_AT_EVERY_8_EVENTS   3

/*
 * input capture x filter
 */
#define SAMPLING_FDTS_NO_FILTER		0
#define SAMPLING_FCK_INT_N_2		1
#define SAMPLING_FCK_INT_N_4		2
#define SAMPLING_FCK_INT_N_8		3
#define SAMPLING_FDTS_DIV_2_N_6		4
#define SAMPLING_FDTS_DIV_2_N_8		5
#define SAMPLING_FDTS_DIV_4_N_6		6
#define SAMPLING_FDTS_DIV_4_N_8     7
#define SAMPLING_FDTS_DIV_8_N_6     8
#define SAMPLING_FDTS_DIV_8_N_8     9
#define SAMPLING_FDTS_DIV_16_N_5    10
#define SAMPLING_FDTS_DIV_16_N_6	11
#define SAMPLING_FDTS_DIV_16_N_8    12
#define SAMPLING_FDTS_DIV_32_N_5    13
#define SAMPLING_FDTS_DIV_32_N_6    14
#define SAMPLING_FDTS_DIV_32_N_8    15




typedef enum{
	TIMER_CHANNEL_1,
	TIMER_CHANNEL_2,
	TIMER_CHANNEL_3,
	TIMER_CHANNEL_4,
}timerx_channel;


typedef struct{
	uint8_t timer_x_channel_number;
	uint8_t clock_source;
	uint8_t dir;
	uint8_t center_aligned_mode;
	uint16_t prescalar;
	uint16_t auto_reload_reg_value;
	uint8_t capture_compare_selection;
	uint8_t output_compare_mode;
	uint8_t polarity;
	uint8_t input_capture_prescaler;
	uint8_t input_capture_filter;
	uint8_t interrupt_en_or_di;
	uint16_t channel_interrupt_en_or_di;
	uint8_t dma_en_or_di;
	uint16_t channel_dma_en_or_di;
	uint16_t channel_to_enable;

}timer1_channel_x_init;



void timer_channel_init(timerx_regdef_t *p_timerx,uint8_t channel);
void start_timer(timerx_regdef_t *p_timerx);
void disable_timer(timerx_regdef_t *p_timerx);
void update_timer_x_channel(timerx_regdef_t *p_timerx, uint8_t channel, uint16_t delay_in_ms);
uint8_t get_timer_flag_status(timerx_regdef_t *p_timerx, uint8_t flag);
#endif /* INC_TIMER_H_ */
