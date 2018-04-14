//ͷ�ļ�����
#include "uart.h"
#include "usually.h"
#include "app_scheduler.h"
#include "lora.h"
#include "timer.h"

#include "test.h"

void init_system(void);
timer_t SYSLED;
timer_t timerLoRa;
void SYSTEM_LED(void* arg);//ϵͳ����ָʾ

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
	
	#ifdef TEST_LORA
	//lora�����շ����Ժ���
	//testLoRa();
	#endif
	
	while(1)													
	{ 
		//�������
		app_sched_execute();
		//USART3_Send_String((uint8_t*)"CHECK WATERMETER CMD",sizeof("CHECK WATERMETER CMD"));
	}
}

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
	Usart2_Configuration(9600);	//�������� ���ò�����Ϊ115200
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
	timer_create(&SYSLED,200,OPT_TMR_PERIODIC,SYSTEM_LED,NULL);
	timer_start(&SYSLED);
	#ifdef DEBUG_MAIN_FUN
	printf("|--init system LED success----|\r\n");
	#endif
	
	#ifdef DEBUG_MAIN_FUN
	printf("|-------**S*Y*S*T*E*M**-------|\r\n");
	printf("+-----------------------------+\r\n\r\n");
	#endif
	
	
	
	#ifdef TEST_LORA
	init_LoRa();
	//lora�����շ����Ժ���
	testLoRa();
	#endif
	
}

void SYSTEM_LED(void* arg)
{
	(void)arg;
	LED3=~LED3;
}






/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
End:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

