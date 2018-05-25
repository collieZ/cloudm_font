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

u16 len;	// 串口一数据长度 

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
	
	//LCD_Init();								//LCD初始化
	InitRc522();
	
	ExitSetup();	//出货电机控制初始化
	
	//LCD_Clear(WHITE);
	TIM3_Int_Init(1999,7199);	//频率10KHZ，溢出时间200ms——温湿度模块
	
	printf("正常启动\r\n");
	/*                                                                               */
	while(1)
	{
//		getIdCard();
//		u3_printf("usart3 123ok");
//		printf("test usart1");
		selectDrugs();
		pushSelectDrugs();	// 出药
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
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			for(t=0;t<len/2-1;t++)
//			{
////				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
////				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//				printf("t的值：%d",t);
//				
//			}
//			printf("haha\r\n\r\n");//插入换行
//			USART_RX_STA=0;
//		}
	
	if(USART_RX_STA&0x8000) {
		
		len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		for (i = 0; i < len/2; i++) {			// 获取购买药品名和数量
			DrugsName[i] = USART_RX_BUF[i*2];
			DrugsNumbers[i] = USART_RX_BUF[i*2+1];
			DrugsCount++;
		}
		  for (i = 0; i < DrugsCount; i++) {
			PushDrugs[i][0] = DrugsName[i];
			PushDrugs[i][1] = DrugsNumbers[i];
		  }
		  pushDrugsNumbers = DrugsCount;		// 确定等会出药数量
	}

	
//	memset(DrugsName,0,sizeof(DrugsName));
//	memset(DrugsNumbers,0,sizeof(DrugsNumbers));
	DrugsCount = 0;		// 选择药品数清零
	USART_RX_STA = 0;
}

void pushSelectDrugs(void) {		// 根据选择的药品进行出货												2018/5/24 完成单个药品选定数量出货、多个药品暂未实现 @@@ 待解决问题 ：getDrugsMsg_flag标志位问题导致的一直置为问题需要解决！！！
	
	static u16 nums = 0;
	
	if(getDrugsMsg_flag == 1){
		
		delay_ms(200);
		pushHowmuchDrugs(nums);		// 确定出货量
		pushWhitchDrugs(PushDrugs[nums][0]);		// 要出的药品名  并出药
	}
	
	if (nums >= pushDrugsNumbers) {
		
	}
}

void pushWhitchDrugs(u8 drugsArr) {			// 确定出货的药品名
	
	switch(drugsArr) {
		
		case 0x41:		// A
			GPIO_SetBits(GPIOB,GPIO_Pin_6);
			getDrugsMsg_flag = 0;		// 防止出货数量标志一直置位
		
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

void pushHowmuchDrugs(u16 drugs) {		// 确定某药品出货量
	
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
