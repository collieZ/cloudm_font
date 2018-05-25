#include "sys.h"
#include "motor_Contral.h"
#include "usart3.h"
#include "usart.h"	
#include "delay.h"

/*==========================================================   
 *�������ƣ�MotorInit()
 *��    �ܣ��Ե�����Ƶ�IO�ڽ��г�ʼ��
 *��ڲ�������
 *���ڲ�������
 *˵    ����
 *==========================================================*/
 
 
 void MotorInit(void){
	 
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTB,PORTEʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	// �ر�PB4 JTAG���� ����Ϊ��ͨio��

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4;//����������  1~4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB 7-4
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4;//����������  5~8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA 7-4
	 
	/*============================================================================================*/
	 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTE,PORTEʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;		//�ⲿ�ж϶�ȡ���io�����ź� PC1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC1  
 }
 
 
 
 /*==========================================================   
 *�������ƣ�ExtixInit()
 *��    �ܣ��Ե�����Ƶ�IO�ڵ��ⲿ�жϽ��г�ʼ��
 *��ڲ�������
 *���ڲ�������
 *˵    ����`
 *==========================================================*/
 
 
void ExtixInit(void){
	 
	EXTI_InitTypeDef EXTI_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	MotorInit();		//������ų�ʼ��
	 
	//GPIOC.1 �ж����Լ��жϳ�ʼ������   �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;	//motor  input
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//�½��ش����ж�
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���  1
 
 }
 
 /*==========================================================   
 *�������ƣ�void EXTI_SetupNVIC(void)
 *��    �ܣ���ʼ������źſ��ж����ȼ�
 *��ڲ�������
 *���ڲ�������
 *˵    ����`
 *==========================================================*/
 
 
void EXTI_SetupNVIC(void){
	
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  
 
 }

 
 /*==========================================================   
 *�������ƣ�void Exit_Setup(void)
 *��    �ܣ��ܳ�ʼ��
 *��ڲ�������
 *���ڲ�������
 *˵    ����`
 *==========================================================*/
 
void ExitSetup(void){
	 
	ExtixInit();	//�ⲿ�жϳ�ʼ��
	EXTI_SetupNVIC();	//�ж����ȼ���ʼ��
	 
}


//�ⲿ�ж�1�������  PC1
int counter = 0;

u8 PushDrugs[5][2];
u32 pushDrugsNumbers;
u32 drugsNum_flag = 66;  // ���������ı�־ 
u16 getDrugsMsg_flag = 1;  // ҩƷѡ���־λ  ѡ��һ�κ�ȴ�������ϲŽ�����һ��ѡ��

void EXTI1_IRQHandler(void){
	
	delay_ms(3);
	if(PCin(1) == 0){
		drugsNum_flag--;
		if(drugsNum_flag <= 0) {
				
				drugsNum_flag = 66;
				GPIOA->BRR = 0x00F0;		//�ر����е��     io�Ĵ��������ο��ԣ�https://blog.csdn.net/stephenbruce/article/details/48392769    http://blog.sina.com.cn/s/blog_640029b30100i33h.html
				GPIOB->BRR = 0X00F0;
				getDrugsMsg_flag = 0;
		}
	}
	
//	if(PCin(1) == 0) {
//		counter++;
//		printf("ok");
//		GPIOA->BRR = 0x00F0;		//�ر����е��     io�Ĵ��������ο��ԣ�https://blog.csdn.net/stephenbruce/article/details/48392769    http://blog.sina.com.cn/s/blog_640029b30100i33h.html
//		GPIOB->BRR = 0X00F0;
//	}
	
	EXTI_ClearITPendingBit(EXTI_Line1); //���LINE1�ϵ��жϱ�־λ  
}


