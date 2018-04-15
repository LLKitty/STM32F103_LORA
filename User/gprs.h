#ifndef __GPRS_H__
#define __GPRS_H__

#include "app_scheduler.h"
#include "loraSet.h"
#include "uart.h"
#include "debug.h"
#include "usually.h"
#include "timer.h"
#include "lora.h"

#define str_Call Ready  "Call Ready\r\n"
#define str_SMS Ready   "SMS Ready\r\n"
#define ATTEST			"AT\r\n"
#define str_CGATT 			"AT+CGATT?\r\n"
#define str_CIPMODE 		"AT+CIPMODE=1\r\n"
#define str_CSTT 				"AT+CSTT=\"UNINET\"\r\n"
#define str_CIICR 			"AT+CIICR\r\n"
#define str_CIFSR 			"AT+CIFSR\r\n"
#define str_CIPSTART 		"AT+CIPSTART=\"TCP\",\"139.199.222.35\",\"2345\"\r\n"


typedef enum GPRS_Power
{
		POWER_OFF = 0,
		POWER_ON  =1,
} GPRS_PowerTypdef;

void GPRS_RecvPacket(uint8_t *data,uint8_t len);
void GPRS_PacketParse(void *data,uint16_t len);

void GPRS_Init(void);
void GPRS_EN_Init(void);
void GPRS_Power(GPRS_PowerTypdef status);
void GPRS_Reset(void);
uint8_t GPRS_SendATCMD(uint8_t *data);
void GPRS_ConnectServer(uint8_t *net,uint8_t *ip,uint8_t *port);
void GPRS_ConnectParse(void *data,uint16_t len);
	

/************************************************************************************/

/*************	���غ�������	**************/
void System_Initialization(void); //STM32ϵͳ��ʼ��
void Ram_Initialization(void);    //�ڴ������ʼ��

void CLR_Buf1(void);              //�崮��1���ջ���
u8 Find(char *a);                 //�����ַ���
void Second_AT_Command(char *b,char *a,u8 wait_time);  //����ATָ��
void Wait_CREG(void);                          //��ѯ�ȴ�ģ��ע��ɹ�
void Set_ATE0(void);                           //ȡ������
void Connect_Server(void);                     //����GPRS���Ӳ�����������͸��ģʽ
void Rec_Server_Data(void);                    //���շ��������ݲ�����


#endif
