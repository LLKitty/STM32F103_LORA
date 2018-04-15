#include "gprs.h"

#define Buf2_Max 200 					  //����1���泤��
/*************  ���ر�������	**************/
char Uart1_Buf[Buf2_Max]; //����2���ջ���
char *p1,*p2;             
u8 Times=0;
/*************	���س�������	**************/
const char *string = "AT+CIPSTART=\"TCP\",\"139.199.222.35\",2345";//IP��¼������,��Ҫ�����Լ���IP�Ͷ˿����޸�


void GPRS_RecvPacket(uint8_t *data,uint8_t len)
{
	if(data==NULL)
		 return;
	uint32_t temp = app_sched_event_put(data, len, GPRS_PacketParse);
	if(temp != 0)
	{
		printf("[error]%s,%d\r\n",__FILE__,__LINE__);
		return;
	}
}

void GPRS_PacketParse(void *data,uint16_t len)
{
	uint8_t i;
	uint8_t info[255] = {0};
	for(i=0;i<len;i++)
	{
		info[i] = ((uint8_t*)data)[i];
	}
	#ifdef DEBUG_GPRS
	
	printf("[GPRS_PacketParse]\r\n");
	
	for(i=0;i<len;i++)
	{
		printf("0x%02x,",((uint8_t*)data)[i]);
		//printf("%c,",((uint8_t*)data)[i]);
	}
	
	printf("----------------------------\r\n");
	#endif
	/*
	if(((uint8_t*)data)[2] == 0x02 && ((uint8_t*)data)[3] == 0x01)
	{
		uint8_t up2master[10] = {0x00,0x01,0x17,0xff,0xff};
		
		#ifdef DEBUG_GPRS
		printf("[MASTER GPRS get watermeter data]");
		#endif
		
		for(i=2;i<len;i++)
		{
			up2master[i+3] = ((uint8_t*)data)[i];
		}
		
		#ifdef DEBUG_GPRS
		printf("\r\n[MASTER GPRS to MASTER watermeter data]");
		printf("\r\n------------------------------------\r\n");
		#endif
		*/
		//�·�����
		if(strstr((char *)info,"CMD"))
		{
			//uint8_t MASTER_SENDTEST[8]={0x00,0x00,0x17,0xff,0xff,0x00,0x01,0x00};
			uint8_t cmd[8] = {0x00,0x00,0x17,0xff,0xff,0x01,0x01,0x00};
			LoRa_SendPacket(cmd,sizeof(cmd));
		}
		
}





void GPRS_Init(void)
{
	GPRS_Reset();
	
	GPRS_EN_Init();

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,DISABLE);
	
	printf("GPRSģ��GPRS���Գ���\r\n");
	printf("GPRSģ����ע������\r\n");
	Delay_Ms(1000); 
	USART3_Send_String((uint8_t *)"+++",3);//�˳�͸��ģʽ������ģ�黹����͸��ģʽ��
  Delay_Ms(2000);
	Wait_CREG();   //�ȴ�ģ��ע��ɹ�
	printf("GPRSģ��ע��ɹ�\r\n");
	printf("GPRSģ�鿪ʼ���ӷ�����\r\n");
	Set_ATE0();    //ȡ������
	Connect_Server(); //����GPRS����
	printf("���ӳɹ�\r\n");
	
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
}

void GPRS_EN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;										//����һ��GPIO�ṹ�����
		
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB,ENABLE);		//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;								//����EN��9PIN
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   		//ͨ���������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   		//���ö˿��ٶ�Ϊ50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);				   				//���ݲ�����ʼ��GPIOD�Ĵ���
}

void GPRS_Power(GPRS_PowerTypdef status)
{
	if(status == POWER_ON)
		GPIO_SetBits(GPIOB,GPIO_Pin_9);
	if(status == POWER_OFF)
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	Delay_Ms(1000);
}

void GPRS_Reset(void)
{
	GPRS_Power(POWER_OFF);
	GPRS_Power(POWER_ON);
}


/*******************************************************************************
* ������  : Ram_Initialization
* ����    : ������ʼ������
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/

void Ram_Initialization(void)
{
	Times=0;
	p1=Uart1_Buf;
	p2=p1;

}

/*******************************************************************************
* ������  : USART2_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/

void USART1_IRQHandler(void)                	
{
	u8 Res=0;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res=USART_ReceiveData(USART1);
		*p1 = Res;  	  //�����յ����ַ����浽������
		p1++;                			//����ָ������ƶ�
		if(p1>&Uart1_Buf[Buf2_Max])       		//���������,������ָ��ָ�򻺴���׵�ַ
		{
			p1=Uart1_Buf;
		} 
	}			
} 	




/*******************************************************************************
* ������ : CLR_Buf1
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //��������������
	{
		Uart1_Buf[k] = 0x00;
	}
  p1=Uart1_Buf;               //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart1_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void AT_timerHandler(void *arg)
{
	if(Times>0)
	{
		Times--;
	}
}
void Second_AT_Command(char *b,char *a,u8 wait_time)         
{
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf1(); 
	Times = wait_time;
	for (; *b!='\0';b++)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
		USART_SendData(USART3,*b);//UART2_SendData(*b);
	}
	printf("CMD = [%s]\r\n",c);
	USART3_Send_String((uint8_t *)"\r\n",2);

	uint8_t ret = Find(a);
	printf("Times = %d,ret = %d\r\n",Times,ret);
	                  
	while(Times && !ret)
	{
		Delay_Ms(1000);
		Times--;
		ret = Find(a);
		printf("Times = %d,ret = %d\r\n",Times,ret);
		if(ret) 
		{
			break;
    }
	}
	Times = 0;
		
	printf("Uart1_Buf = [%s]\r\n",Uart1_Buf);
	
	CLR_Buf1(); 
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
  while(i == 0)        			
	{
		CLR_Buf1();        
		USART3_Send_String((uint8_t*)"AT+CREG?",strlen("AT+CREG?"));   //��ѯ�ȴ�ģ��ע��ɹ�
		USART3_Send_String((uint8_t*)"\r\n",2);
		Delay_Ms(5000);  						
	    for(k=0;k<Buf2_Max;k++)      			
    	{
				if(Uart1_Buf[k] == ':')
				{
					if((Uart1_Buf[k+4] == '1')||(Uart1_Buf[k+4] == '5'))   //˵��ע��ɹ�
					{
						i = 1;
						printf("\r\n");
						break;
					}
				}
			}
		printf("ע����.....");
	}
}

/*******************************************************************************
* ������ : Set_ATE0
* ����   : ȡ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3);								//ȡ������	
	printf("ȡ������\r\n");	
}
/*******************************************************************************
* ������ : Connect_Server
* ����   : GPRS���ӷ���������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Connect_Server(void)
{
	USART3_Send_String((uint8_t *)"AT+CIPCLOSE=1",strlen("AT+CIPCLOSE=1"));	//�ر�����
  Delay_Ms(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//�ر��ƶ�����
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ���	
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"UNINET\"","OK",2);//����PDP������,��������Э��,��������Ϣ
	Second_AT_Command("AT+CGATT=1","OK",2);//����GPRSҵ��
	Second_AT_Command("AT+CIPCSGP=1,\"UNINET\"","OK",2);//����ΪGPRS����ģʽ
  Second_AT_Command("AT+CIPMUX=0","OK",2);//����Ϊ��·����
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
	Second_AT_Command("AT+CIPMODE=1","OK",2);//��͸������
	Second_AT_Command("AT+CIPCCFG=4,5,200,1","OK",2);//����͸��ģʽ�������ط�����:2,���1S����һ��,ÿ�η���200���ֽ�
  Second_AT_Command((char*)string,"OK",5);//��������
  Delay_Ms(100);                                //�ȴ��������ݽ������
  CLR_Buf1();                                    //�崮�ڽ��ջ���Ϊ͸��ģʽ׼��
}

















/*******************************************************************************
* ������  : System_Initialization
* ����    : STM32ϵͳ��ʼ������(��ʼ��STM32ʱ�Ӽ�����)
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
/*
void System_Initialization(void)
{
	RCC_Configuration();		//����ϵͳʱ��Ϊ72MHZ(������Ը�����Ҫ��)
  SysTick_Init_Config();  //��ʼ��ϵͳ�δ�ʱ��SysTick
  NVIC_Configuration();		//STM32�ж�������������
	Timer2_Init_Config();		//��ʱ��2��ʼ������
	USART1_Init_Config(115200);	//����1��ʼ������
	USART2_Init_Config(115200);	//����2��ʼ������	
	GPIO_Config();          //��ʼ��GPIO
}
*/

/*******************************************************************************
* ������ : main 
* ����   : ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : ����2������MG323ģ��ͨ�ţ�����1���ڴ��ڵ��ԣ����Ա��������س���ʱ����
					 �����͵�ģ��
*******************************************************************************/
/*
int main(void)
{
	System_Initialization(); //ϵͳ��ʼ��
	Ram_Initialization();    //������ʼ��

	

	while(1)
	{
		Rec_Server_Data();//�������ݲ�ԭ������	
	}
}
*/

/*******************************************************************************
* ������ : Rec_Server_Data
* ����   : ���շ��������ݺ���,��ԭ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
/*
void Rec_Server_Data(void)
{
	if(p2!=p1)   		//˵����������δ����
	{	
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		USART_SendData(USART2,*p2);
		p2++;
    if(p2>&Uart1_Buf[Buf2_Max])
			p2=Uart1_Buf;
	}
}
*/

