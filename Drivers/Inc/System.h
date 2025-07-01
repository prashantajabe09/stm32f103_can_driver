/*
 * System.h
 *
 *  Created on: 11-Sep-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "stm32f103x.h"

void nvic_en_irq(irqn_type irqn);
void systick_init(void);
#endif /* INC_SYSTEM_H_ */
