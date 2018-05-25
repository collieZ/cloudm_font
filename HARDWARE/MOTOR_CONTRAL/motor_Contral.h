#ifndef	_Moto_col_H
#define _Moto_col_H
#include "sys.h"

void ExitSetup(void);	//电机返回口及外部中断初始化

extern  u8 PushDrugs[5][2];
extern u32 pushDrugsNumbers;
extern u32 drugsNum_flag;
extern u16 getDrugsMsg_flag;

#endif


