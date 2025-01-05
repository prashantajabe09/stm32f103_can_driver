/*
 * Timer.c
 *
 *  Created on: 22-May-2024
 *      Author: Prashant Ajabe
 */
#include "Timer.h"

uint16_t capture_value;
uint8_t flag_send_data;
timer1_channel_x_init timer1_channel_x_config[4] = {
		[TIMER_CHANNEL_1] = {TIMER_CHANNEL_1,0,UP_COUNTING,CENTER_ALIGNED_MODE_3,8000,10,CC_X_AS_OUTPUT,PWM_MODE_1,CC_X_ACTIVE_HIGH,NOT_APPLICABLE,NOT_APPLICABLE,DISABLE,TIM_DIER_CC1I_EN,DISABLE,TIM_DIER_CC1DE_EN,CAPTURE_COMPARE_1_OUTPUT_EN},
		[TIMER_CHANNEL_2] = {TIMER_CHANNEL_2,0,UP_COUNTING,EDGE_ALIGNED_MODE,8000,10,CC_X_AS_INPUT_IC_X_TO_TI_X,NOT_APPLICABLE,IC_X_RISING,NO_PRESCALER,SAMPLING_FCK_INT_N_8,ENABLE,TIM_DIER_CC2I_EN,DISABLE,TIM_DIER_CC1DE_EN,CAPTURE_COMPARE_2_OUTPUT_EN},
};

void timer_channel_init(timerx_regdef_t *p_timerx,uint8_t channel)
{
	uint8_t temp;

	//Center aligned mode selection
	p_timerx->CR1 &= ~(MASK_FOR_CLEAR_2_BIT << CMS_BIT_POSTION_CR1);
	p_timerx->CR1 |= timer1_channel_x_config[channel].center_aligned_mode << CMS_BIT_POSTION_CR1;

	//Counting  Direction
	if (timer1_channel_x_config[channel].dir == UP_COUNTING)
		p_timerx->CR1 &= ~(1 << COUNTING_DIR_BIT_POSITION_CR1);
	else
		p_timerx->CR1 |= (1 << COUNTING_DIR_BIT_POSITION_CR1);

	//Pre-scalar
	p_timerx->PSC = timer1_channel_x_config[channel].prescalar;

	p_timerx->ARR = timer1_channel_x_config[channel].auto_reload_reg_value;

	// CC_X configuration as output or input
	temp = timer1_channel_x_config[channel].timer_x_channel_number % 2;
	p_timerx->CCMR[timer1_channel_x_config[channel].timer_x_channel_number / 2] &= ~(MASK_FOR_CLEAR_2_BIT << (8 * temp));
	p_timerx->CCMR[timer1_channel_x_config[channel].timer_x_channel_number / 2] |= (timer1_channel_x_config[channel].capture_compare_selection << (8 * temp));

	//output compare mode
	if (timer1_channel_x_config[channel].capture_compare_selection == CC_X_AS_OUTPUT)
	{
		temp = timer1_channel_x_config[channel].timer_x_channel_number % 2;
		p_timerx->CCMR[timer1_channel_x_config[channel].timer_x_channel_number / 2] &= ~(MASK_FOR_CLEAR_3_BIT << ((8 * temp) + 4));
		p_timerx->CCMR[timer1_channel_x_config[channel].timer_x_channel_number / 2] |= (timer1_channel_x_config[channel].output_compare_mode << ((8 * temp) + 4));
		p_timerx->BDTR |= (1 << MAIN_OUTPUT_EN_BIT_POSITION_BDTR);
	}
	if (timer1_channel_x_config[channel].capture_compare_selection != CC_X_AS_OUTPUT)
	{
		// input capture prescaler
		temp = timer1_channel_x_config[channel].timer_x_channel_number % 2;
		p_timerx->CCMR[timer1_channel_x_config[channel].timer_x_channel_number / 2] &= ~(MASK_FOR_CLEAR_2_BIT << ((8 * temp) + 2));
		p_timerx->CCMR[timer1_channel_x_config[channel].timer_x_channel_number / 2] |= (timer1_channel_x_config[channel].input_capture_prescaler << ((8 * temp) + 2));
		//input capture filter
		temp = timer1_channel_x_config[channel].timer_x_channel_number % 2;
		p_timerx->CCMR[timer1_channel_x_config[channel].timer_x_channel_number / 2] &= ~(MASK_FOR_CLEAR_4_BIT << ((8 * temp) + 4));
		p_timerx->CCMR[timer1_channel_x_config[channel].timer_x_channel_number / 2] |= (timer1_channel_x_config[channel].input_capture_filter << ((8 * temp) + 4));
		//
	}


	//
	//interrupt en or di
	if (timer1_channel_x_config[channel].interrupt_en_or_di == ENABLE)
	{
		p_timerx->DIER |= timer1_channel_x_config[channel].channel_interrupt_en_or_di;
		nvic_en_irq(TIM1_CC_IRQn);
	}
	else
	{
		p_timerx->DIER &= ~timer1_channel_x_config[channel].channel_interrupt_en_or_di;
	}
	//dma en or di
	if (timer1_channel_x_config[channel].dma_en_or_di == ENABLE)
		p_timerx->DIER |= timer1_channel_x_config[channel].channel_dma_en_or_di;
	else
		p_timerx->DIER &= ~timer1_channel_x_config[channel].channel_dma_en_or_di;

	//timer channel active high polarity
	p_timerx->CCER &= ~(timer1_channel_x_config[channel].polarity << OC_1_POLARITY_BIT_POSITION_CCER);

	//timer channel EN
	p_timerx->CCER |= timer1_channel_x_config[channel].channel_to_enable;

}



void update_timer_x_channel(timerx_regdef_t *p_timerx, uint8_t channel, uint16_t delay_in_ms)
{
	p_timerx->CCR[timer1_channel_x_config[channel].timer_x_channel_number] = delay_in_ms;
}
void start_timer(timerx_regdef_t *p_timerx)
{
	//p_timerx->CCR[timer1_channel_x_config[channel - 1].timer_x_channel_number] = delay_in_ms * 10;
	p_timerx->CR1 |= (1 << COUNTER_EN_BIT_POSTION_CR1);
}

void disable_timer(timerx_regdef_t *p_timerx)
{
	p_timerx->CR1 &= (1 << COUNTER_EN_BIT_POSTION_CR1);
}

uint8_t get_timer_flag_status(timerx_regdef_t *p_timerx, uint8_t flag)
{
	if (p_timerx->SR & flag)
		return 1;
	else
		return 0;
}
void TIM1_CC_IRQHandler(void)
{
	if (get_timer_flag_status(TIMER1,TIM_DIER_CC2I_EN))
	{
		// clear the interrupt
		TIMER1->SR &= ~(1 << 2);
		// read CCR
		capture_value = TIMER1->CCR[1];
		flag_send_data = 1;
		// set flag to print
	}
}
