#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  
      

#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern __IO u8 aRxBuffer[8];
extern __IO uint8_t rxCounter;
extern __IO uint8_t ReceiveState;


extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART3_RX_STA;   						//��������״̬
void motoroutput_Init(void);
void usart3_init(u32 bound);				//����2��ʼ�� 
void u3_printf(char* fmt,...);
#endif













