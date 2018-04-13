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
	
	//LCD_Init();								//LCD初始化
	InitRc522();
	
	ExitSetup();	//出货电机控制初始化
	
	//LCD_Clear(WHITE);
	TIM3_Int_Init(1999,7199);	//频率10KHZ，溢出时间200ms——温湿度模块
	
	
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

