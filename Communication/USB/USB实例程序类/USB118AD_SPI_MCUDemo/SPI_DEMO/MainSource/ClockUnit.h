//=====================================================================================================================
//	����Ԫ��װ��һ��ʱ�ӺͶ�ʱ��
//=====================================================================================================================
#ifndef	__Clock_Unit_H
#define	__Clock_Unit_H

#include	"Config.h"



//=====================================================================================================================
//	����Ķ�ʱ�����
#define	cCLK_TimerCommand	0x00
#define	cCLK_TimerBlink		0x01
#define	cCLK_TimerDelay		0x02
#define	cCLK_MaxTimer		0x03
//=====================================================================================================================


//ĳ��Timer��Ӧ�ĺ�����
typedef	void	tCLK_OnTimer	(void);




//��ʼ��Timer0���̶���ʱ���Ϊ5ms��
void	CLK_Init(void);

//��ʼ��ĳһ����ŵ�Timer
void	CLK_SetupTimer(uint32	WhichTimer,	uint32	Interval,	tCLK_OnTimer*	UserOnTimer);

//�ж��Ƿ�ĳ����ŵ�Timer�Ƿ�Ӧ��ִ��
BOOL	CLK_OnTimer(uint32	WhichTimer);

//�ӳ�һ��ʱ��
void	CLK_Delay(uint32	WhichTimer,	uint32	Interval);


#endif

