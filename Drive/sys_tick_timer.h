#ifndef __SYS_TICK_TIMER_H__
#define __SYS_TICK_TIMER_H__

#include "stm32f10x.h"
#include "core_cm3.h"   //uint32_t SysTick_Config(uint32_t ticks)
#include "misc.h"       //void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)



/*----��һ������---��δ�ʱ�ӼĴ���д��ֵ��Ҳ����Ҫ����ϵͳƵ�����ݼ�----Ȼ��Ϊ0 ������ж�*/

void SysTick_Init(uint32_t a);   //a-us

void PreciseDelay_us(__IO u32 nTime);

void PreciseDelay_ms(__IO u32 nTime);

void TimingDelay_Decrement(void);   //���ĵݼ�---���ж���ʹ��

#endif


/*
void PreciseDelay_us(__IO uint32_t us);
void PreciseDelay_ms(__IO uint32_t ms);
void PreciseDelay_s(__IO uint32_t s);
*/

