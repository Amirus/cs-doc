//=====================================================================================================================
/*
	����Ԫʵ����һ����ʱ������ʱ��λΪ1ms��
*/
//=====================================================================================================================

#include	"Config.h"
#include	"ClockUnit.h"
#include	"IRQ_Unit.h"

//	����Ԫ�ڲ�ʹ�õĺ궨�塣
//=====================================================================================================================
//=====================================================================================================================


//	����Ԫ�ڲ�ʹ�õ�ȫ�ֱ�����
//=====================================================================================================================

//��ʱ��ʹ�õ���ȫ�ֱ���
struct	tCLK_TimerRec
{
	uint32	Interval;
	uint32	LastTime;
	tCLK_OnTimer*	UserOnTimer;
};

struct	tCLK_TimerRec	vCLK_TimerRec[cCLK_MaxTimer];
uint32	vCLK_TimerCount;	//��ʱ���ļ�������ÿ1ms��1��
//=====================================================================================================================





//*********************************************************************************************************************
/*

		Timer0���жϷ������

*/
//=====================================================================================================================
void  IRQ_Timer0(void)  __irq
{
	int	i;

	//��������1
	vCLK_TimerCount++;

	for	(i=0;	i<cCLK_MaxTimer;	i++)
	{
		//���Timer�����жϷ�ʽ�������ǲ�ѯ��ʽ�Ļ���
		if	(vCLK_TimerRec[i].UserOnTimer	!=	0)
		{
			uint32	NowInterval,	NeedInterval;
			NowInterval	=	vCLK_TimerCount	-	vCLK_TimerRec[i].LastTime;
			NeedInterval	=	vCLK_TimerRec[i].Interval;
			if	(NowInterval	>=	NeedInterval)
			{
				if	(NowInterval	<	(NeedInterval	<<	1))
				{
					vCLK_TimerRec[i].LastTime	=	vCLK_TimerRec[i].LastTime	+	NeedInterval;
				}
				else
				{
					vCLK_TimerRec[i].LastTime	=	vCLK_TimerCount;
				}
				(*(vCLK_TimerRec[i].UserOnTimer))();
			}
		}
	}

	T0IR		=	0x01;					//	����жϱ�־
	VICVectAddr	=	0x00;					//	�жϴ������
}
//=====================================================================================================================






//*********************************************************************************************************************
/*

		��ʼ��Timer0��

��ڲ�����
	WhichIntSlot	����� Timer0	ʹ�õ��жϺš�
*/
//=====================================================================================================================
void	CLK_Init(void)
{
	int	i;

	vCLK_TimerCount	=	0;
	for	(i=0;	i<cCLK_MaxTimer;	i++)
	{
		vCLK_TimerRec[i].Interval	=	1000;
		vCLK_TimerRec[i].LastTime	=	0;
		vCLK_TimerRec[i].UserOnTimer	=	0;
	}

	
	//#define Fosc            24M                    //Crystal frequence,10MHz~25MHz��should be the same as actual status. 
	//#define Fcclk           (Fosc *  2) = 48M                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
	//#define Fpclk           (Fcclk / 4) * 1   =  12M          //VPB clock frequence , must be 1��2��4 multiples of (Fcclk / 4).
	//��ʼ��Ϊ1ms��ʱ���
	T0PR	=	119;		//	���ö�ʱ��0��ƵΪ120��Ƶ����100,000Hz
	T0MCR	=	0x03;		//	ƥ��ͨ��0ƥ���жϲ���λT0TC
	T0MR0	=	100;		//	�Ƚ�ֵ100(1ms��ʱֵ)
	T0TCR	=	0x03;		//	��������λT0TC
	T0TCR	=	0x01;

	//��ʼ���ж�
	IRQ_SetInt(cIRQ_Timer0,	0,	(DWORD)IRQ_Timer0);
}
//=====================================================================================================================






//*********************************************************************************************************************
/*

		��ʼ��Timerͨ����

��ڲ�����
	WhichIntTimer	�ĸ�Timer��
	Interval		Timer��ʱ�������msΪ��λ��
	UserOnTimer		�û��Ķ�ʱ��Ӧ���������ֵΪ0�����ʾ���ò�ѯ��ʽ��
*/
//=====================================================================================================================
void	CLK_SetupTimer(uint32	WhichTimer,	uint32	Interval,	tCLK_OnTimer*	UserOnTimer)
{
	if	(Interval	==	0)
		Interval	=	1;
	

	vCLK_TimerRec[WhichTimer].Interval	=	Interval;
	vCLK_TimerRec[WhichTimer].LastTime	=	vCLK_TimerCount;
	vCLK_TimerRec[WhichTimer].UserOnTimer	=	UserOnTimer;
}
//=====================================================================================================================




//*********************************************************************************************************************
/*

		�ж��Ƿ�ĳ����ŵ�Timer�Ƿ�Ӧ��ִ�С�

��ڲ�����
	WhichIntTimer	�ĸ�Timer��
���ڲ�����
	�棺��Timer�Ѿ�Ӧ�ñ�������
*/
//=====================================================================================================================
BOOL	CLK_OnTimer(uint32	WhichTimer)
{
	uint32	NowInterval,	NeedInterval,	NowCount;

	NowCount	=	vCLK_TimerCount;
	NowInterval	=	NowCount	-	vCLK_TimerRec[WhichTimer].LastTime;
	NeedInterval	=	vCLK_TimerRec[WhichTimer].Interval;
	if	(NowInterval	>=	NeedInterval)
	{
		if	(NowInterval	<	(NeedInterval	<<	1))
		{
			vCLK_TimerRec[WhichTimer].LastTime	=	vCLK_TimerRec[WhichTimer].LastTime	+	NeedInterval;
		}
		else
		{
			vCLK_TimerRec[WhichTimer].LastTime	=	NowCount;
		}
		return	TRUE;
	}

	return	FALSE;
}
//=====================================================================================================================


//*********************************************************************************************************************
/*

		�ӳ�һ��ʱ�䡣

��ڲ�����
	WhichIntTimer	�ĸ�Timer��
���ڲ�����
	�棺��Timer�Ѿ�Ӧ�ñ�������
*/
//=====================================================================================================================
void	CLK_Delay(uint32	WhichTimer,	uint32	Interval)
{
	CLK_SetupTimer(WhichTimer,	Interval,	NULL);
	while	(!CLK_OnTimer(WhichTimer))
	{}
}
//=====================================================================================================================


