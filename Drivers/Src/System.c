/*
 * System.c
 *
 *  Created on: 11-Sep-2024
 *      Author: Prashant Ajabe
 */

#include "System.h"

void nvic_en_irq(irqn_type irqn)
{
	uint8_t register_position = irqn / 32;
	uint8_t irq_position = irqn % 32;
	NVIC->ISER[register_position] = (1 << irq_position);
}
