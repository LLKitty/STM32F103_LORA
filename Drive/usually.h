#ifndef __USUALLY_H
#define __USUALLY_H

//ͷ�ļ�����
#include "stm32f10x.h"
#include "hardware.h"

//LED���Ŷ���
#define custom_log(model,level,arg,...) \
		do{printf("[%s][%s][%s:%d]" arg "\r\n", #level, model, __FUNCTION__,__LINE__,  ##__VA_ARGS__);}while(0)

#define LED3 PCout(13)	   	//�����϶�Ӧ��LED3


void Delay_Ms(uint16_t time);  
void Delay_Us(uint16_t time); 
void Init_LED3(void);	
#endif
