#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "timer.h"
#include "adc.h"
#include "usart3.h"
#include "rc522.h"
#include "motor_Contral.h"
#include "lcd.h"


void getIdCard(void);

int main(void)
{
	/*   var   rfid      */
//	u8 ret = 0;
//	char card[9] = {0};
	
	/*        init        */
	delay_init();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	uart_init(115200);
	
	usart3_init(9600);
	
	//LCD_Init();								//LCD��ʼ��
	InitRc522();
	
	ExitSetup();	//����������Ƴ�ʼ��
	
	//LCD_Clear(WHITE);
	TIM3_Int_Init(1999,7199);	//Ƶ��10KHZ�����ʱ��200ms������ʪ��ģ��
	
	
	/*                                                                               */
	while(1)
	{
		getIdCard();
		delay_ms(400);
		
	}
	
}


void getIdCard(void){
	
	ReadID();
	Compar_Card(num);
}

