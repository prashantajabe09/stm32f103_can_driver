/*
 * iwdg.h
 *
 *  Created on: 09-Jan-2025
 *      Author: Prashant Ajabe
 */

#ifndef INC_IWDG_H_
#define INC_IWDG_H_

#include "stm32f103x.h"

void iwdg_init();
void iwdg_start();
void rl_update();
void iwdg_update();

#endif /* INC_IWDG_H_ */
