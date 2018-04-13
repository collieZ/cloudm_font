 #include "adc.h"
 #include "delay.h"
 #include "usart.h"

   
__IO uint16_t ADC_ConvertedValue;		//����ֵ����
float ADC_ConvertedValueLocal; 	//����������ֵ

void MQ_2_AverageValue(float value);

//��ʼ��ADC																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); //ʹ��MNA1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA0 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA1_Channel1); //��λdma1
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)ADC1_DR_Address;	//Դ��ַ  ADC��ַ   PeripheralBaseAddr �ⲿ����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue; //�ڴ��ַ(Ҫ����ı�����ָ��)  MemoryBaseAddr�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //���䷽������  ����
	DMA_InitStructure.DMA_BufferSize = 1;	//���仺������С 1 �������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�ڴ��ͺ��Ƿ����� 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�浽�ڴ���ַ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;    //�ڴ����ݵ�λ    ���ֽ�  16λ 
	DMA_InitStructure.DMA_PeripheralDataSize =  DMA_PeripheralDataSize_HalfWord ; //�������ݵ�λ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ; //DMAģʽ��ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; //���ȼ�����  
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   //��ֹ�ڴ浽�ڴ�Ĵ��� 
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA1��1ͨ��  
	
	DMA_Cmd(DMA1_Channel1,ENABLE);  //ʹ��DMA
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE); //��������ж�
	
	
	ADC_DeInit(ADC1);  //��λADC1 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5); //����ADC1ͨ��0Ϊ239����������  
	
	ADC_DMACmd(ADC1,ENABLE);  //ADC DMAʹ��
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
    
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼   
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 


float ADC_RealValueLocal = 0;
float MQ_2Concentration =300; //MQ-2Ũ�� ppm��λ ��ʼ300


//============================================
//��������:DMA1_Channel1_IRQHandler
//��������:DMA�ж� ��AD�ɼ�ֵƽ���˲�
//����:��
//���:��
//============================================
void DMA1_Channel0_IRQHandler(void)		//DMA�ж�
{
	
	if(DMA_GetITStatus(DMA1_IT_TC1)!=0)
	{
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue*3.3/4096; // ֵת��
		MQ_2_AverageValue(ADC_ConvertedValueLocal);
		printf("����");
//		if(MQ_2Concentration>=2700)
//		{
//			
//		}
	}
		DMA_ClearITPendingBit(DMA1_IT_TC1);
		DMA_ClearFlag(DMA1_FLAG_TC1);
}


void MQ_2_AverageValue(float value)		//��ֵ�˲�
{
	static float temp_val= 0;
	static u8 TIMES = 0;
	
	TIMES++;
	temp_val+=value;
	if(TIMES >= 20)
	{
		ADC_RealValueLocal = temp_val/TIMES;
		if(ADC_RealValueLocal<=0.15)
		{
			ADC_RealValueLocal=0.15;
		}
		MQ_2Concentration = (ADC_RealValueLocal-0.15)*100*20+300;		//ppm��λ
		TIMES = 0;	
		temp_val=0;
	}
}




















