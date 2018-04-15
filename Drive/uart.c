#include "uart.h"
#include "usually.h"

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int Data, FILE *f)
{   
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus���õ�����״̬λ
														  //USART_FLAG_TXE:���ͼĴ���Ϊ�� 1��Ϊ�գ�0��æ״̬
	USART_SendData(USART1,Data);						  //����һ���ַ�
	   
	return Data;										  //����һ��ֵ
}
#endif 

/***************************************************************/


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
USART1**********************************************************
���Դ���*********************************************************
RX-->PA9********************************************************
TX-->PA10*******************************************************
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART1_Send_Byte
** ��������: ���ڷ���һ���ַ�
** ����������Data Ҫ���͵�����
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_Send_Byte(uint8_t Data)
{ 
	USART_SendData(USART1,Data);//��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){}//�ȴ����ͽ�
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART1_Send_String
** ��������: ���ڷ���һ���ַ���
** ����������Data Ҫ���͵�����
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_Send_String(uint8_t* Data,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		USART1_Send_Byte(Data[i]);
		//while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	}
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART1_Send_Byte
** ��������: ���ڷ���һ���ַ���
** ����������Data Ҫ���͵�����
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_Receive_Byte(uint8_t len)
{ 
	;
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_NVIC
** ��������: ϵͳ�ж�����
** ������������
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_NVIC_USART1(void)
{ 	
  	NVIC_InitTypeDef NVIC_InitStructure;			//����һ��NVIC������ṹ�����

	#ifdef  VECT_TAB_RAM  							//���������ַѡ��

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//��0x20000000��ַ��Ϊ���������ַ(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //��0x08000000��ַ��Ϊ���������ַ(FLASH)  
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж��� Ϊ2 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//���ô���1Ϊ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//����ռ�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//���ø����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//ʹ�ܴ���1�ж�
	NVIC_Init(&NVIC_InitStructure);							  	//���ݲ�����ʼ���жϼĴ���
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Usart_Init
** ��������: �������ų�ʼ��
** ��������: ��
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_Usart1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 							//RX-->PA9
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//���ù��������©
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   			//���ݲ�����ʼ��GPIOA�Ĵ���	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 							//TX-->PA10
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������(��λ״̬);	   				
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   			//���ݲ�����ʼ��GPIOA�Ĵ���	
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Usart1_Configuration
** ��������: �������ú���
** ��������: BaudRate���ò����� 
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Usart1_Configuration(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;							    							//����һ�����ڽṹ��

	USART_InitStructure.USART_BaudRate            =BaudRate ;	  					//������115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 	//���������ʹ��8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 		//��֡��β����1λֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 		//��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ����ʧ��
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //���պͷ���ģʽ
	USART_Init(USART1, &USART_InitStructure);															//���ݲ�����ʼ�����ڼĴ���
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);													//ʹ�ܴ����жϽ���
	USART_Cmd(USART1, ENABLE);     																				//ʹ�ܴ�������
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART1_IRQHandler
** ��������: �����жϺ���
** ��������: �� 
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
//uint8_t USART1_ReceiveDate[64],USART1_LEN=0;;								//����һ��������Ž��յ�����
/*
void USART1_IRQHandler()
{
	uint8_t RX_dat;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//�жϷ��������ж�
	{
		RX_dat= USART_ReceiveData(USART1) ;      //�������ݣ������ȥǰ��λ
	}
	USART_ClearITPendingBit(USART1,  USART_IT_RXNE);          //����жϱ�־
	//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}//�ȴ����ս���
		
	USART_SendData(USART1 ,RX_dat);             //�������� 
}

*/

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
USART2*********************************************************
LoRa���ݶ˿�****************************************************
RX-->PA3*******************************************************
TX-->PA2*******************************************************
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
#define MAX_UART_LEN  255
uint8_t UART2_RECV_BUF[MAX_UART_LEN]={0};
uint8_t UART2_RECV_LEN=0;
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART2_Send_Byte
** ��������: ���ڷ���һ���ַ���
** ����������Data Ҫ���͵�����
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART2_Send_Byte(uint8_t Data)
{ 
	USART_SendData(USART2,Data);//��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}//�ȴ����ͽ�
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART1_Send_String
** ��������: ���ڷ���һ���ַ���
** ����������Data Ҫ���͵�����
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART2_Send_String(uint8_t* Data,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		USART2_Send_Byte(Data[i]);
	}
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART2_Send_Byte
** ��������: ���ڷ���һ���ַ���
** ����������Data Ҫ���͵�����
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*
#define MAX_UART_LEN  64
uint8_t UART2_RECV_BUF[MAX_UART_LEN]={0};
uint8_t UART2_RECV_LEN=0;
*/
void USART2_Receive_DMA(uint8_t* data,uint8_t len)
{
	//data = UART2_RECV_BUF
	;
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_NVIC_USART2
** ��������: ϵͳ�ж�����
** ������������
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_NVIC_USART2(void)
{ 	
  	NVIC_InitTypeDef NVIC_InitStructure;			//����һ��NVIC������ṹ�����

	#ifdef  VECT_TAB_RAM  							//���������ַѡ��

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//��0x20000000��ַ��Ϊ���������ַ(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //��0x08000000��ַ��Ϊ���������ַ(FLASH)  
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж��� Ϊ2

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//���ô���2Ϊ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//����ռ�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//���ø����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//ʹ�ܴ���2�ж�
	NVIC_Init(&NVIC_InitStructure);							  	//���ݲ�����ʼ���жϼĴ���
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_DMA_USART2
** ��������: ����2 DMA����
** ��������: ��
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


void Init_DMA_USART2(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&UART2_RECV_BUF[0]);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = MAX_UART_LEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6,ENABLE);

	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Usart_Init
** ��������: �������ų�ʼ��
** ��������: ��
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_Usart2(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 							//TX-->PA2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//���ù��������©
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   			//���ݲ�����ʼ��GPIOA�Ĵ���	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 							//RX-->PA3
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������(��λ״̬);	   				
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   			//���ݲ�����ʼ��GPIOA�Ĵ���	
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Usart2_Configuration
** ��������: �������ú���
** ��������: BaudRate���ò����� 
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Usart2_Configuration(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;							    							//����һ�����ڽṹ��

	USART_InitStructure.USART_BaudRate            =BaudRate ;	  					//������115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 	//���������ʹ��8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 		//��֡��β����1λֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 		//��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ����ʧ��
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //���պͷ���ģʽ
	USART_Init(USART2, &USART_InitStructure);															//���ݲ�����ʼ�����ڼĴ��� 
  
	//DMA���俪���������
	USART_ITConfig(USART2,USART_IT_TC,DISABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE); 
  USART_Cmd(USART2, ENABLE);	// ʹ��USART2
	USART_ClearFlag(USART2, USART_FLAG_TC);     //�巢����Ǳ�־��Transmission Complete flag   CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	Init_DMA_USART2();
	
	//��ʹ��DMA���俪���������
	//USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);													//ʹ�ܴ����жϽ���
	//USART_Cmd(USART2, ENABLE);     																				//ʹ�ܴ�������
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART2_IRQHandler
** ��������: �����жϺ���
** ��������: �� 
** ��  ����: 
** �ա�  ��: 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

//#include "test.h"
#include"debug.h"
extern void LoRa_RecvPacket(uint8_t *data,uint8_t len);
void USART2_IRQHandler()
{
	//��ʹ��DMA�жϽ��պ������ļ�ĩβע�Ͳ���
	//������ʹ��DMA�жϽ��ܺ���
	uint32_t temp = 0;
	//printf("*******************************************\r\n");
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
  {
		USART_ClearFlag(USART2,USART_IT_IDLE);
    temp = USART2->SR;
    temp = USART2->DR; 
		temp=temp;
    DMA_Cmd(DMA1_Channel6,DISABLE);
    UART2_RECV_LEN = MAX_UART_LEN - DMA_GetCurrDataCounter(DMA1_Channel6);
		
		#ifdef DEBUG_USART2_IRQHANDLER
		uint8_t i;
		#endif
		
    if(UART2_RECV_LEN>3)// && UART2_RECV_BUF[0] == LEADBYTE && UART2_RECV_BUF[1] == LEADBYTE  )
    {
			#ifdef DEBUG_USART2_IRQHANDLER
			printf("[USART2 DMA RECV data info:]");
			for(i=0;i<UART2_RECV_LEN;i++)
			{
				printf("%02x ",UART2_RECV_BUF[i]);
			}
			printf("____\r\n");
			#endif
			//LoRa���ݴ���
			LoRa_RecvPacket(UART2_RECV_BUF,UART2_RECV_LEN);
    }
		            
    DMA_SetCurrDataCounter(DMA1_Channel6, MAX_UART_LEN);
    DMA_Cmd(DMA1_Channel6,ENABLE);
	}
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
USART3*********************************************************
***************************************************************
RX-->PB11******************************************************
TX-->PB10******************************************************
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

uint8_t UART3_RECV_BUF[MAX_UART_LEN]={0};
uint8_t UART3_RECV_LEN=0;

void USART3_Send_Byte(uint8_t Data)
{
	USART_SendData(USART3,Data);//��������
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET){}//�ȴ����ͽ�
}
void USART3_Send_String(uint8_t* Data,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		USART3_Send_Byte(Data[i]);
	}
}
void USART3_Receive_Byte(uint8_t len)
{
	
}
void USART3_Receive_DMA(uint8_t* data,uint8_t len)
{
	;
}
void Init_Usart3(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO ,ENABLE);
  //GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE); 
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
  //GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE); 
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 							//TX-->PB10
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//���ù��������©
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOB, &GPIO_InitStructure);				   			//���ݲ�����ʼ��GPIOA�Ĵ���	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 							//RX-->PB11
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������(��λ״̬);	   				
  	GPIO_Init(GPIOB, &GPIO_InitStructure);				   			//���ݲ�����ʼ��GPIOA�Ĵ���	
}
void Usart3_Configuration(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;							    							//����һ�����ڽṹ��

	USART_InitStructure.USART_BaudRate            =BaudRate ;	  					//������115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 	//���������ʹ��8λ����
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 		//��֡��β����1λֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 		//��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ����ʧ��
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //���պͷ���ģʽ
	USART_Init(USART3, &USART_InitStructure);															//���ݲ�����ʼ�����ڼĴ���
	
	//DMA���俪���������
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
  USART_Cmd(USART3, ENABLE);	// ʹ��USART3
	USART_ClearFlag(USART3, USART_FLAG_TC);     //�巢����Ǳ�־��Transmission Complete flag   CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	Init_DMA_USART3();
	
	//������DMA�����������
	//USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);													//ʹ�ܴ����жϽ���
	//USART_Cmd(USART3, ENABLE);     																				//ʹ�ܴ�������
}
void Init_NVIC_USART3(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;			//����һ��NVIC������ṹ�����

	#ifdef  VECT_TAB_RAM  							//���������ַѡ��

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//��0x20000000��ַ��Ϊ���������ַ(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //��0x08000000��ַ��Ϊ���������ַ(FLASH)  
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж��� Ϊ2

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			//���ô���2Ϊ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 	//����ռ�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//���ø����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//ʹ�ܴ���2�ж�
	NVIC_Init(&NVIC_InitStructure);							  	//���ݲ�����ʼ���жϼĴ���
}
void Init_DMA_USART3(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&UART3_RECV_BUF[0]);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = MAX_UART_LEN;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel3,ENABLE);

	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
}

#include "test.h"

extern void MBUS_RecvPacket(uint8_t *data,uint8_t len);
extern void GPRS_RecvPacket(uint8_t *data,uint8_t len);
void USART3_IRQHandler()
{
	//��ʹ��DMA�жϽ��պ������ļ�ĩβע�Ͳ���
	/*
	uint8_t RX_dat;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�жϷ��������ж�
	{
		RX_dat=(USART_ReceiveData(USART3) & 0x7F);      //�������ݣ������ȥǰ��λ
	}
	RX_dat=(USART_ReceiveData(USART3) & 0x7F);
	USART_ClearITPendingBit(USART3,USART_IT_RXNE);          //����жϱ�־
	USART_SendData(USART3,RX_dat);             //��������
	*/
	
	//������ʹ��DMA�жϽ��ܺ���
	
	uint32_t temp = 0;
	//printf("*******************************************\r\n");
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
  {
		USART_ClearFlag(USART3,USART_IT_IDLE);
    temp = USART3->SR;
    temp = USART3->DR; 
		temp=temp;
    DMA_Cmd(DMA1_Channel3,DISABLE);
    UART3_RECV_LEN = MAX_UART_LEN - DMA_GetCurrDataCounter(DMA1_Channel3);

		#ifdef DEBUG_USART3_IRQHANDLER
		uint8_t i;
		#endif
		
    if(UART3_RECV_LEN>3)// && UART3_RECV_BUF[0] == LEADBYTE && UART3_RECV_BUF[1] == LEADBYTE  )
    {
			#ifdef DEBUG_USART3_IRQHANDLER
			printf("[USART3 DMA RECV data info:]\r\n[");
			printf("%s\r\n",UART3_RECV_BUF);
			for(i=0;i<UART3_RECV_LEN;i++)
			{
				//printf("%02x ",UART3_RECV_BUF[i]);
				printf("%c",UART3_RECV_BUF[i]);
			}
			printf("]____\r\n");
			#endif
			
			//MBUS���ݴ��ݲ���
			//test_get_data(UART3_RECV_BUF,UART3_RECV_LEN);
			#ifdef MBUS
			MBUS_RecvPacket(UART3_RECV_BUF,UART3_RECV_LEN);
			#endif
			
			#ifdef GPRS
			GPRS_RecvPacket(UART3_RECV_BUF,UART3_RECV_LEN);
			#endif
    }
		            
    DMA_SetCurrDataCounter(DMA1_Channel3, MAX_UART_LEN);
    DMA_Cmd(DMA1_Channel3,ENABLE);
	}
	
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
End:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/



/**************************************************************
**********��ʹ��DMAʱ��USART2���жϽ��պ���**********************
***************************************************************
void USART2_IRQHandler()
{
	uint8_t RX_dat;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�жϷ��������ж�
	{
		RX_dat=(USART_ReceiveData(USART2) & 0x7F);      //�������ݣ������ȥǰ��λ
	}
	RX_dat=(USART_ReceiveData(USART2) & 0x7F);
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);          //����жϱ�־
	USART_SendData(USART2,RX_dat);             //��������
}
***************************************************************
**********��ʹ��DMAʱ��USART2���жϽ��պ���**********************
**************************************************************/
