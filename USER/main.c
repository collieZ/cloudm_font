#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "timer.h"
#include "adc.h"
#include "usart3.h"
#include "rc522.h"
#include "motor_Contral.h"
#include "lcd.h"
#include <string.h>


void getIdCard(void);
void selectDrugs(void);
void pushWhitchDrugs(u8 drugsArr);
void pushHowmuchDrugs(u16 drugs);
void pushSelectDrugs(void);

u16 len;	// ����һ���ݳ��� 

u8 DrugsName[10];
u8 DrugsNumbers[10];
int DrugsCount = 0;

int main(void)
{
	/*   var   rfid      */
//	u8 ret = 0;
//	char card[9] = {0};
	/*        init        */
	

	
	delay_init();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	uart_init(9600);
	
	usart3_init(9600);
	
	//LCD_Init();								//LCD��ʼ��
	InitRc522();
	
	ExitSetup();	//����������Ƴ�ʼ��
	
	//LCD_Clear(WHITE);
	TIM3_Int_Init(1999,7199);	//Ƶ��10KHZ�����ʱ��200ms������ʪ��ģ��
	
	printf("��������\r\n");
	/*                                                                               */
	while(1)
	{
//		getIdCard();
//		u3_printf("usart3 123ok");
//		printf("test usart1");
		selectDrugs();
		pushSelectDrugs();	// ��ҩ
		delay_ms(50);
	}
	
}


void getIdCard(void){
	
	ReadID();
	Compar_Card(num);
}

void selectDrugs(void) {
	int i; 

//	int t;
//	if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			for(t=0;t<len/2-1;t++)
//			{
////				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
////				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//				printf("t��ֵ��%d",t);
//				
//			}
//			printf("haha\r\n\r\n");//���뻻��
//			USART_RX_STA=0;
//		}
	
	if(USART_RX_STA&0x8000) {
		
		len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		for (i = 0; i < len/2; i++) {			// ��ȡ����ҩƷ��������
			DrugsName[i] = USART_RX_BUF[i*2];
			DrugsNumbers[i] = USART_RX_BUF[i*2+1];
			DrugsCount++;
		}
		  for (i = 0; i < DrugsCount; i++) {
			PushDrugs[i][0] = DrugsName[i];
			PushDrugs[i][1] = DrugsNumbers[i];
		  }
		  pushDrugsNumbers = DrugsCount;		// ȷ���Ȼ��ҩ����
	}

	
//	memset(DrugsName,0,sizeof(DrugsName));
//	memset(DrugsNumbers,0,sizeof(DrugsNumbers));
	DrugsCount = 0;		// ѡ��ҩƷ������
	USART_RX_STA = 0;
}

void pushSelectDrugs(void) {		// ����ѡ���ҩƷ���г���												2018/5/24 ��ɵ���ҩƷѡ���������������ҩƷ��δʵ�� @@@ ��������� ��getDrugsMsg_flag��־λ���⵼�µ�һֱ��Ϊ������Ҫ���������
	
	static u16 nums = 0;
	
	if(getDrugsMsg_flag == 1){
		
		delay_ms(200);
		pushHowmuchDrugs(nums);		// ȷ��������
		pushWhitchDrugs(PushDrugs[nums][0]);		// Ҫ����ҩƷ��  ����ҩ
	}
	
	if (nums >= pushDrugsNumbers) {
		
	}
}

void pushWhitchDrugs(u8 drugsArr) {			// ȷ��������ҩƷ��
	
	switch(drugsArr) {
		
		case 0x41:		// A
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			getDrugsMsg_flag = 0;		// ��ֹ����������־һֱ��λ
		
		break;
		
		case 0x42:		// B
			
		break;
		
		case 0x43:
			
		break;
		
		case 0x44:
			
		break;
		
		case 0x45:
			
		break;
		
		case 0x46:
			
		break;
		
		case 0x47:
			
		break;
		
		case 0x48:
			
		break;
		
		default:
			
		break;
	}
}

void pushHowmuchDrugs(u16 drugs) {		// ȷ��ĳҩƷ������
	
	if (PushDrugs[drugs][1] == 0x31) {
		drugsNum_flag = 1;
	}
	else if (PushDrugs[drugs][1] == 0x32) {
		drugsNum_flag = 2;
	}
	else if (PushDrugs[drugs][1] == 0x33) {
		drugsNum_flag = 3;
	}
	else if (PushDrugs[drugs][1] == 0x34) {
		drugsNum_flag = 4;
	}
	else if (PushDrugs[drugs][1] == 0x35) {
		drugsNum_flag = 5;
	}
}
