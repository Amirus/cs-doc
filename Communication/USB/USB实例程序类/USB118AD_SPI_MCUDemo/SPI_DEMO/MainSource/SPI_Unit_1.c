//============================================================================================================
//
//
//	����Ԫ�Ĺ���������
//
//			ʵ��	ͨ��UART1	�������ݵĹ��ܡ�
//
//
//	��������б�
//
//			UART1_Init		��ʼ��
//			UART1_Send		��������
//===========================================================================================================
//	����	|	����	|	����	|	����	
//===========================================================================================================
//2008-07-28|	���� 	|	������	|	�����ļ� 
//============================================================================================================

//============================================================================================================
//		����ͷ�ļ�
//============================================================================================================
//#include	<LPC23xx.h>                        /* LPC23xx/24xx definitions */

#include	"Config.h"
#include	"IRQ_Unit.h"
#include	"SPI_unit.h"

//============================================================================================================
//����һЩ����
//============================================================================================================

//============================================================================================================
//����һЩȫ�ֱ�������Щȫ�ֱ�����Command��Ԫ���塣
//============================================================================================================
//����һЩȫ�ֱ���
BYTE	vSPI_FirstReadData;
BYTE	vSPI_TempData;
WORD	vSPI_ReadCounter;
volatile	BYTE*	vSPI_RD_pBufPos;
volatile	BYTE*	vSPI_RD_pBufStart;
volatile	BYTE*	vSPI_RD_pBufEnd;
volatile	BYTE*	vSPI_WR_pBufPos;
volatile	BYTE*	vSPI_WR_pBufStart;
volatile	BYTE*	vSPI_WR_pBufEnd;
//============================================================================================================



//============================================================================================================
//************************************************************************************************************
//*
//*
//*			��ʼ����	����UART�жϡ�ʵʩ���ݴ����	������
//*
//*
//************************************************************************************************************
//============================================================================================================
//��������		UART1_ReadBuffer
//
//���ù�ϵ��	��	SPI���жϴ������SPI_Handler	�������á�
//
//����������	��RBR��ȡ���ݣ�����������ݻ�������
//
//				�ڱ������У��޸�Command��Ԫ�ж����ȫ�ֱ���  ���������ݻ�����ָ�룩vSPI_RD_pBufPos	��ֵ��
//
//				�õ��µ����ݺ󣬵���Command��Ԫ��һ��������ͨ��Command��Ԫȥ������Щ���յ������ݡ�
//
//��ڲ�����	�ޡ�
//
//����ֵ��		�ޡ�
//============================================================================================================
void SPI_ReadBuffer (void)
{
	while	(SSPSR	&	SSPSR_RNE)
	{
		//��������
		*vSPI_RD_pBufPos	=	SSPDR;
		if	((vSPI_RD_pBufPos	==	vSPI_RD_pBufEnd)	&&	
			(vSPI_FirstReadData)	&&	(vSPI_FirstReadData	==	*vSPI_RD_pBufPos))
		{
			*vSPI_RD_pBufPos	=	0;
		}
		/*if	((vSPI_ReadCounter	==	0)	&&	(vSPI_RD_pBufPos	!=	vSPI_RD_pBufEnd)	&&	
			(vSPI_FirstReadData)	&&	(vSPI_FirstReadData	!=	*vSPI_RD_pBufPos))
		{
			*vSPI_RD_pBufPos	=	0;
		}*/
		//����ǽ��ܵ�һ�����ݣ�����Ҫ�ȵ���һ��������������Ҫ������֮�󣬲Ž��н���
		if	((vSPI_ReadCounter	==	0)	&&	(vSPI_FirstReadData))
		{
			if	(*vSPI_RD_pBufPos	==	vSPI_FirstReadData)
			{
				//����������FIFO��ա��͡�����FIFO���������ж�
				//SSPIMSC	=	SSPIMSC_RORIM | SSPIMSC_RTIM | SSPIMSC_RXIM | SSPIMSC_TXIM;
				//vSPI_FirstReadData++;
				if	(vSPI_RD_pBufPos	==	vSPI_RD_pBufEnd)
				{
					vSPI_FirstReadData++;
					continue;
				}
			}
			else
			{	continue;	}
		}
		//�ƶ�����ָ��
		vSPI_ReadCounter++;
		vSPI_RD_pBufPos++;
		if	(vSPI_RD_pBufPos	>=	vSPI_RD_pBufEnd)
		{
			vSPI_RD_pBufPos	=	(BYTE*)&vSPI_TempData;
			vSPI_RD_pBufStart	=	vSPI_RD_pBufPos;
			vSPI_RD_pBufEnd	=	vSPI_RD_pBufPos;
			vSPI_FirstReadData	=	0;
			//���û�ж�������Ҫ��ȡ����ֹͣ������FIFO���������ж�
			//SSPIMSC	&=	~SSPIMSC_RXIM;
			//return;
			continue;
		}
	}
}
//============================================================================================================
//��������		SPI_WriteBuffer
//
//���ù�ϵ��	��	SPI���жϴ������SPI_Handler	�������á�
//
//����������	�ѷ������ݻ������е����ݷ��͵�THR�Ĵ�����
//	
//				����������TX��FIFOΪ��ʱ������ִ�У��ں����У����д8���ֽڵ����ݵ�TX��FIFO�С�
//
//				�ڱ������У��޸��ڲ�ȫ�ֱ���  ���������ݻ�����ָ�룩vSPI_WR_pLastPos	��ֵ��
//
//��ڲ�����	�ޡ�
//
//����ֵ��		�ޡ�
//============================================================================================================
void SPI_WriteBuffer (void)
{
	DWORD	i;
	i	=	0;
	//ѭ������
	while	(SSPSR	&	SSPSR_TNF)
	{
		if	(vSPI_WR_pBufPos	==	0)
		{
		//����ǹ����ڴ�ģʽ������Ҫ�����µĹ���
			/*SSP0DR	=	0xFFFF;*/
			
		//����ǹ�������ģʽ������Ҫ�����µĹ���
			//������е����ݶ��Ѿ������꣬��ô���
			if	(vSPI_FirstReadData	==	0)
			{	//���û�ж�������Ҫ��ȡ����ֹͣ������FIFO��ա����ж�
				SSPIMSC	&=	~SSPIMSC_TXIM;
				return;	
			}
			else
			{	//�������������Ҫ��ȡ������0xFF���ü���ִ�ж�ȡ������
				SSPDR	=	0xFFFF;
				i++;
				if	(i	>	3)
					return;	
			}
		}
		else
		{
			//���û��������Ҫ���ͣ���ָ��ָ��ȱʡ��ַ
			if	(vSPI_WR_pBufPos	>=	vSPI_WR_pBufEnd)
			{
				vSPI_WR_pBufPos	=	0;
				SSPIMSC	&=	~SSPIMSC_TXIM;
				//�����������FIFO��׼�������豸��Ӧ��
				while	(SSPSR	&	SSPSR_RNE)
				{
					i	=	SSPDR;
				}
				return;	
			}
			//��������
			SSPDR	=	*vSPI_WR_pBufPos;
			//�ƶ�����ָ��
			vSPI_WR_pBufPos++;
			i++;
			if	(i	>	3)
				return;	
		}
	}
}
//============================================================================================================
//��������		SPI_Handler
//
//���ù�ϵ��	��	Ӳ���ж�	���á�
//
//����������	SPI1���жϴ������
//
//��ڲ�����	�ޡ�
//
//����ֵ��		�ޡ�
//============================================================================================================
void SPI_Handler (void) __irq 
{
	WORD	IntCond;
	volatile	BYTE Dummy;

	IntCond	=	SSPMIS;
	if ( IntCond & SSPMIS_RORMIS )	/* Receive overrun interrupt */
	{
		SSPICR = SSPICR_RORIC;		/* clear interrupt */
	}
	if ( IntCond & SSPMIS_RTMIS )	/* Receive timeout interrupt */
	{
		SSPICR = SSPICR_RTIC;		/* clear interrupt */
	}

	if ( IntCond & SSPMIS_RXMIS )	/* Rx at least half full */
	{
		SPI_ReadBuffer();		/* receive until it's empty */		
	}

	if ( IntCond & SSPMIS_TXMIS )	/* Tx at least half empty */
	{
		SPI_WriteBuffer();		/* send until it's empty */		
	}

    VICVectAddr = 0;		// Acknowledge Interrupt
}
//============================================================================================================
//************************************************************************************************************
//*
//*
//*			��ʼ����	����Ԫ���ⲿ�ӿں���	������
//*
//*
//************************************************************************************************************
//============================================================================================================
//��������		SPI_Init
//
//���ù�ϵ��	��	�ⲿ����	�������á�
//
//����������	SPI1�ĳ�ʼ������
//
//��ڲ�����	��
//
//����ֵ��		�ޡ�
//============================================================================================================
BOOL	SPI_Init( void )
{
	vSPI_ReadCounter	=	0;
	vSPI_RD_pBufPos	=	(BYTE*)&vSPI_TempData;
	vSPI_RD_pBufStart	=	vSPI_RD_pBufPos;
	vSPI_RD_pBufEnd	=	vSPI_RD_pBufPos;
	vSPI_WR_pBufPos	=	0;
	vSPI_WR_pBufStart	=	vSPI_RD_pBufPos;
	vSPI_WR_pBufEnd	=	vSPI_RD_pBufPos;

	PINSEL0	|=	0x20000000;       /* SCK1 */
	PINSEL1	|=	0x00000540;       /* SSEL1,MOSI1,MOSI1 */
	
	// enable clock to SSP1 for security reason. By default, it's enabled already
	PCONP	|=	(1 << 10);

	//Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15
	SSPCR0	=	0x0107;

	// SSPCPSR clock prescale register, minimum divisor is 0x02 in master mode
	SSPCPSR	=	0x2;

	IRQ_SetInt( cIRQ_SPI1, 1, (DWORD)SPI_Handler );
	
	// Device select as master, SSP Enabled
	SSPCR1	=	SSPCR1_SSE;
	//* Set SSPINMS registers to enable interrupts */
	//* enable all error related interrupts */
	SSPIMSC	=	SSPIMSC_RORIM | SSPIMSC_RTIM;

	return	TRUE;
}
//============================================================================================================
//��������		SPI_Send
//
//���ù�ϵ��	��	�ⲿ����	�������á�
//
//����������	���÷������ݻ�������ָ�룬Ȼ����д��һ�ε�THR���Ժ�����ݶ����жϷ�������б���д��THR�����͡�
//
//��ڲ�����	pBufStart	���ͻ���������ʼλ�á�
//				DataLength	��Ҫ���͵����ݳ��ȡ�
//
//����ֵ��		�ޡ�
//============================================================================================================
BOOL	SPI_Send(void*	pBufStart,	DWORD	DataLength)
{
	if	((vSPI_WR_pBufPos	<	vSPI_WR_pBufEnd)	&&	(vSPI_WR_pBufPos	!=	0))
	{
		return	FALSE;
	}

	IRQ_Disable(	cIRQ_SPI1	);

	vSPI_WR_pBufStart	=	pBufStart;
	vSPI_WR_pBufEnd	=	(BYTE*)((DWORD)pBufStart	+	DataLength);
	vSPI_WR_pBufPos	=	vSPI_WR_pBufStart;

	SSPIMSC	|=	SSPIMSC_TXIM;
	SPI_WriteBuffer();

	IRQ_Enable(	cIRQ_SPI1	);

	return	TRUE;
}
//============================================================================================================
//��������		SPI_CanSendNextPacket
//
//���ù�ϵ��	��	�ⲿ����	�������á�
//
//����������	��ѯ�Ƿ���Է�����һ�����ݰ���
//
//��ڲ�����	��
//
//����ֵ��		���дָ����ڷ������ݴ���������λ�ã��򷵻��棻���򷵻ؼ١�
//============================================================================================================
BOOL	SPI_CanSendNextPacket(void)
{
	return	(vSPI_WR_pBufPos	>=	vSPI_WR_pBufEnd)	||	(vSPI_WR_pBufPos	==	0);
}
//============================================================================================================
//��������		SPI_StartRead
//
//���ù�ϵ��	��	�ⲿ����	�������á�
//
//����������	���������ݹ��̡�
//
//��ڲ�����	pBufStart	���ͻ���������ʼλ�á�
//				DataLength	��Ҫ���յ����ݳ��ȡ�
//
//����ֵ��		�ޡ�
//============================================================================================================
void	SPI_StartRead(void*	pBufStart,	DWORD	DataLength,	DWORD	FirstData)
{
	IRQ_Disable(	cIRQ_SPI1	);

	vSPI_RD_pBufStart	=	pBufStart;
	vSPI_RD_pBufEnd	=	(BYTE*)((DWORD)pBufStart	+	DataLength);
	vSPI_RD_pBufPos	=	pBufStart;

	vSPI_FirstReadData	=	FirstData;
	vSPI_ReadCounter	=	0;

	SSPIMSC	|=	SSPIMSC_RXIM;

	IRQ_Enable(	cIRQ_SPI1	);
}
//============================================================================================================
//��������		SPI_GetReadCounter
//
//���ù�ϵ��	��	�ⲿ����	�������á�
//
//����������	��ѯ���˶��ٸ����ݡ�
//
//��ڲ�����	��
//
//����ֵ��		���˶��ٸ����ݡ�
//============================================================================================================
DWORD	SPI_GetReadCounter(void)
{
	if	(SPI_CanSendNextPacket())
	{
		SPI_ReadBuffer();
		SPI_WriteBuffer();
	}

	return	vSPI_ReadCounter;
}
//============================================================================================================
//��������		SPI_CancelRead
//
//���ù�ϵ��	��	�ⲿ����	�������á�
//
//����������	��ֹ�����ݹ��̡�
//
//��ڲ�����	��
//
//����ֵ��		�ޡ�
//============================================================================================================
void	SPI_CancelRead(void)
{
	IRQ_Disable(	cIRQ_SPI1	);

	vSPI_RD_pBufPos	=	(BYTE*)&vSPI_TempData;
	vSPI_RD_pBufStart	=	vSPI_RD_pBufPos;
	vSPI_RD_pBufEnd	=	vSPI_RD_pBufPos;
	vSPI_ReadCounter	=	0;
	vSPI_FirstReadData	=	0;
	SSPIMSC	&=	~SSPIMSC_RXIM;

	IRQ_Enable(	cIRQ_SPI1	);
}
//============================================================================================================




//============================================================================================================
//
//			End	of	File
//
//============================================================================================================
