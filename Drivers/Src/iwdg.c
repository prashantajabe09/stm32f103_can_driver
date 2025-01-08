/*
 * iwdg.c
 *
 *  Created on: 09-Jan-2025
 *      Author: Propix
 */

#include "iwdg.h"
void iwdg_init()
{
	IWDG->iwdg_kr = 0x5555;
	IWDG->iwdg_pr = 0x04;
}
void iwdg_start()
{
	IWDG->iwdg_kr = 0xCCCC;
}
void rl_update()
{
	IWDG->iwdg_kr = 0x5555;
	IWDG->iwdg_rlr = 0xFFF;
}

void iwdg_update()
{
	IWDG->iwdg_kr = 0xAAAA;
}
