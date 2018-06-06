//ͷ�ļ�����
#include "uart.h"
#include "usually.h"
#include "app_scheduler.h"
#include "lora.h"
#include "timer.h"
#include "gprs.h"
#include "flash.h"

#include "test.h"

void init_system(void);
timer_t SYSLED;
timer_t timerLoRa;
void SYSTEM_LED(void* arg);//ϵͳ����ָʾ


#define MAIN_MENU "\r\n\
+--------------------+\r\n\
|                    |\r\n\
|    LoRa testing    |\r\n\
|                    |\r\n\
|--------------------|\r\n\
|--------------------|\r\n"
#define DEVICE_TYPE_MASTER "|*******MASTER*******|\r\n|--------------------|\r\n+--------------------+\r\n"
#define DEVICE_TYPE_SLAVER "|*******SLAVER*******|\r\n|--------------------|\r\n+--------------------+\r\n\r\n"



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: main
** ��������: ���������
** �������������������ʾ�˴����շ����� �������ǲ����жϷ�ʽ
				�������������ϵ�����ֵ ���Գ����ն˽���ʮ����������������ֵ 
				���磺1��Ӧʮ������49��2��Ӧ��50��a��Ӧ��97������㶮������
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


//extern void timerLoRaHandler(void* arg);
void MBUSTEST(void* arg)
{
	(void)arg;
	//MBUS_Send5Bytes();
	printf("############\n");
	//timerLoRaHandler(NULL);
}

int  main()
{
	/*
	SystemInit();					//ϵͳʱ������
	sys_timer_init();
	while(1)
	{
		sys_timer_handle();
	}
	*/
	//ϵͳ��ʼ��
	init_system();
	
	//timer����
	#ifdef TEST_TIMER
	printf("begin test timer. . . . . .\r\n");
	testTimer();
	#endif
	
	//timer_t mbusTest;
	//timer_create(&mbusTest,3000,OPT_TMR_PERIODIC,MBUSTEST,NULL);
	//timer_start(&mbusTest);
	
	
	
	
	GPRS_Power(POWER_ON);
	
	while(1)													
	{ 
		//�������
		app_sched_execute();
	}
	
}
uint32_t tedata[1024] = {0};

void init_system(void)
{
	SystemInit();					//ϵͳʱ������

	Init_NVIC_USART1();					//�ж�������ע�ắ��
	Init_Usart1();					//������������
	Usart1_Configuration(9600);	//�������� ���ò�����Ϊ115200
	#ifdef DEBUG_MAIN_FUN
	printf("\r\n\r\n\r\n");
	printf("+-----------------------------+\r\n");
	printf("|-------**S*Y*S*T*E*M**-------|\r\n");
	printf("|--init usart1 success--------| \r\n");	
	#endif

	Init_NVIC_USART2();					//�ж�������ע�ắ��
	Init_Usart2();					//������������
	Usart2_Configuration(9600);	//�������� ���ò�����Ϊ2400
	#ifdef DEBUG_MAIN_FUN
	printf("|--init usart2 success--------|\r\n");
	#endif
	
	Init_NVIC_USART3();					//�ж�������ע�ắ��
	Init_Usart3();					//������������
	Usart3_Configuration(9600);	//�������� ���ò�����Ϊ115200
	#ifdef DEBUG_MAIN_FUN
	printf("|--init usart3 success--------|\r\n");
	#endif
	
	scheduler_init();
	#ifdef DEBUG_MAIN_FUN
	printf("|--init app scheduler success-|\r\n");
	#endif
	
	timer_init();
	#ifdef DEBUG_MAIN_FUN
	printf("|--init timer success---------|\r\n");
	#endif
	
	Init_LED3();
	timer_create(&SYSLED,200,OPT_TMR_PERIODIC,SYSTEM_LED,NULL);//200 modify
	timer_start(&SYSLED);
	#ifdef DEBUG_MAIN_FUN
	printf("|--init system LED success----|\r\n");
	#endif
	
	#ifdef DEBUG_MAIN_FUN
	printf("|-------**S*Y*S*T*E*M**-------|\r\n");
	printf("+-----------------------------+\r\n\r\n");
	#endif
	
	/*********************************************************************************************************************************************/
	
	uint8_t Temp_Data[1024] = {0};
	uint8_t d[1024] = {0};
	uint32_t data[5] = {0x12345678,0x12345678,0x12345678,0x12345678,0x12345678};
	
  uint32_t ReadNum = 0;
	uint32_t WriteNum = 0;
  int i = 0;  

	
	
	for(i=0;i<1024;i++)
	{
		tedata[i] = i;
		d[i] = i+1;
		//printf("%d,%d\r\n",i,tedata[i]);
	}
	
	
  WriteNum = WriteFlashNWord(0,d,1023);     //д������     
	printf("%d-------------------------------------------------------\r\n",WriteNum);                //���Ͷ�ȡ���ֽ���
  ReadNum = ReadFlashNBtye(0, Temp_Data,1024); //��ȡ����
  printf("%d-------------------------------------------------------\r\n",ReadNum);                //���Ͷ�ȡ���ֽ���  

  

  for(i = 0;i < ReadNum;i++)                
  {
		printf("%x\r\n",Temp_Data[i]);   //���Ͷ�ȡ�������ݣ��ɹ۲����ݵĴ洢��ʽ  
  }
	printf("----------------------------------------------------------\r\n");

	return;
	/*********************************************************************************************************************************************/
	//GPRS_Init();
	//return;
	
	/**************************LoRa ��ʼ��*********************************/
	printf("%s",MAIN_MENU);
	#ifdef MASTER
	printf("%s",DEVICE_TYPE_MASTER);
	#endif
	#ifdef SLAVER
	printf("%s",DEVICE_TYPE_SLAVER);
	#endif
	
	LoRa_Init();
	LoRa_Reset();
	printf("waiting lora for reseting . . . . . .\r\n");
	printf("init LoRa success \r\n\r\n");
	
	PreciseDelay_ms(1000);
	LoRa_GetParameter();
	PreciseDelay_ms(1000);
	LoRa_GetVersionInfo();
	PreciseDelay_ms(1000);
	LoRa_SetMode(MODE0);
	

	LoRaHeartbeat();

	
/*
	GPRS_Init();
*/




}

void SYSTEM_LED(void* arg)
{
	(void)arg;
	LED3=~LED3;
	LED_ADJ4=~LED_ADJ4;
	//USART3_Send_String((uint8_t*)"MBUS MASTER\r\n",sizeof("MBUS MASTER\r\n"));
}






/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
End:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

