#include "sys.h"
#include "MQ_2.h"

#define Mq_2 PFin(0)

void MQ_2(void){
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);	 //ʹ��PF�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);					
}

u8 checkMq_2(void){
	
	if(Mq_2 == 1){
	
		//����
		return 1;
	} else{
		
		//����
		return 0;
	}
}

