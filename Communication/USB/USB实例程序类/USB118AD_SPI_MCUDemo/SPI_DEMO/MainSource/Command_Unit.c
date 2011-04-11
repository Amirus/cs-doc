//============================================================================================================
//
//
//	����Ԫ�Ĺ���������
//
//			ʵ�ֵĹ��ܡ�
//
//
//	��������б�
//			CMD_IdleSetEvent		��	Idle	������ã����ڵ����յ�����ʱ��Command�����˯���л��ѡ�
//
//===========================================================================================================
//	����	|	����	|   	��λ       	|	����	  |��ϵ��ʽ
//===========================================================================================================
//2008-07-28|	���� 	|	 ������̩����  	|	�����ļ�  |029-85272421  http://www.dataie.com
//============================================================================================================

//============================================================================================================
//		����ͷ�ļ�
//============================================================================================================
#include	"Config.h"
#include	"ClockUnit.h"
#include	"SPI_Unit.h"
#include	"Command_Unit.h"


//============================================================================================================
//����һЩ�ڲ���ȫ�ֱ���
volatile	TCMD_CommandBlock	vCMD_CmdStaBuffer;
PCMD_CommandBlock	vCMD_pCommand;
PCMD_StatusBlock	vCMD_pStatus;
//============================================================================================================



//============================================================================================================
//************************************************************************************************************
//*
//*
//*			��ʼ����	�ڲ�����	������
//*
//*
//************************************************************************************************************
//============================================================================================================
//��������		CMD_Init
//
//���ù�ϵ��	��	Command_Task	�������á�
//
//����������	��ʼ������Ԫ��
//
//���ڲ�����	WriteBuffer		�ϲ������õ�д�ļ���������
//				ReadBuffer		�ϲ������õĶ��ļ���������
//
//����ֵ��		�ޡ�
//============================================================================================================
void	CMD_Init(DWORD*	WriteBuffer,	DWORD*	ReadBuffer)
{
	//�������ã���ʼ��ͨѶģ��	Mem_AllocateUSBRAM
	SPI_Init();

	vCMD_pCommand	=	(PCMD_CommandBlock)&vCMD_CmdStaBuffer;
	vCMD_pStatus	=	(PCMD_StatusBlock)&vCMD_CmdStaBuffer;

	*WriteBuffer	=	(DWORD)&(vCMD_pCommand->Data[0]);
	*ReadBuffer		=	(DWORD)&(vCMD_pStatus->Data[0]);
}
//============================================================================================================
//��������		CMD_DoCommand
//
//���ù�ϵ��	��	�����Command	�������á�
//
//����������	ִ��һ�����
//
//��ڲ�����	�ޡ�
//
//����ֵ��		�Ƿ�ɹ���
//============================================================================================================
BOOL	CMD_DoCommand(DWORD	CommandDataLength,	DWORD	ResDataLength,	DWORD	ResponseCode)
{
	WORD	LastCounter,	ThisCounter;

	vCMD_pCommand->Signature	=	cCMD_Signature;

	//���ȣ���������
	if	(!SPI_Send(vCMD_pCommand,	CommandDataLength))
		return	FALSE;
 	
	//�ȴ�������ꡣ
	while	(!SPI_CanSendNextPacket())
	{}

	//Ȼ����������Ӧ��Ĺ���
	SPI_StartRead(vCMD_pStatus,	ResDataLength,	ResponseCode);

	//�ȴ�����Ӧ�����ݳɹ�
	LastCounter	=	0;
	CLK_SetupTimer(cCLK_TimerCommand,	5000,	NULL);
	while	(1)
	{
		ThisCounter	=	SPI_GetReadCounter();
		if	(ThisCounter	>	LastCounter)
		{
			LastCounter	=	ThisCounter;
			CLK_SetupTimer(cCLK_TimerCommand,	500,	NULL);
			//������������ݴ��ڵ�����Ҫ��Ӧ�����ݳ��ȣ���˵��Ӧ��ɹ�
			if	(ThisCounter	>=	ResDataLength)
				return	TRUE;
		}
		else
			if	(CLK_OnTimer(cCLK_TimerCommand))
			{
				return	FALSE;
			}
	}
}
//============================================================================================================
//��������		CMD_WriteFile
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	д�ļ���
//
//��ڲ�����	DataLength	Ҫд�����ݳ���
//
//����ֵ��		�Ƿ�д�ɹ���
//============================================================================================================
BOOL	CMD_WriteFile(	DWORD	DataLength	)
{
	vCMD_pCommand->Parameter[1]	=	DataLength;
	vCMD_pCommand->Parameter[2]	=	DataLength>>8;

	vCMD_pCommand->Command	=	cCMD_CMD_WRITE_FILE;

	if	(!CMD_DoCommand(66+	DataLength,	64,	cCMD_CMD_WRITE_FILE_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_WRITE_FILE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_ReadFile
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	���ļ���
//
//��ڲ�����	DataLength	Ҫ�������ݳ���
//
//����ֵ��		�Ƿ���ɹ���
//============================================================================================================
BOOL	CMD_ReadFile(	DWORD*	DataLength	)
{
	WORD	i,	j;

	vCMD_pCommand->Parameter[0]	=	*DataLength;
	vCMD_pCommand->Parameter[1]	=	*DataLength>>8;

	vCMD_pCommand->Command	=	cCMD_CMD_READ_FILE;

	CMD_DoCommand(66,	64	+	*DataLength,	cCMD_CMD_READ_FILE_RSP);
	i	=	SPI_GetReadCounter();
	if	(i	<	63)
	{	return	FALSE;	}
	else
	{
		if	((vCMD_pStatus->Command	==	cCMD_CMD_READ_FILE_RSP)	&&	(vCMD_pStatus->Result))
		{
			j	=	vCMD_pStatus->Parameter[0]	+	vCMD_pStatus->Parameter[1]	*	256;
			if	(i	>=	(j+64))
			{
				*DataLength	=	j;
				return	TRUE;
			}
			else
			{	return	FALSE;	}
		}
		else
		{	return	FALSE;	}
	}
}
//============================================================================================================
//��������		CMD_CreateFile
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	�����ļ���
//
//��ڲ�����	pFileName	�ļ���������8���ֽ�
//				pExtName	��չ��������3���ֽ�
//
//����ֵ��		�Ƿ񴴽��ɹ���
//============================================================================================================
BOOL	CMD_CreateFile(	char*	pFileName,	char*	pExtName	)
{
	PFS_DirItem	pItem;
	DWORD	i;

	pItem	=	(PFS_DirItem)&(vCMD_pCommand->Parameter[0]);

	//���
	for	(i=0;	i<11;	i++)
	{
		vCMD_pCommand->Parameter[i]	=	0x20;
	}
	for	(i=12;	i<63;	i++)
	{
		vCMD_pCommand->Parameter[i]	=	0;
	}

	//�����ļ�������չ��
	for	(i=0;	i<8;	i++)
	{
		if	(pFileName[i]	==	0)
			break;
		pItem->FileName[i]	=	pFileName[i];
	}
	for	(i=0;	i<3;	i++)
	{
		pItem->ExtName[i]	=	pExtName[i];
		if	(pExtName[i]	==	0)
			break;
	}

	pItem->Attribute		=	cFS_ATTR_ARCHIVE;
	pItem->ResForNT			=	0x0;
	pItem->CreateTimeTenth	=	88;
	pItem->CreateTime		=	0x4104;
	pItem->CreateDate		=	0x3908;
	pItem->LastAccDate		=	0x3908;
	pItem->StartClusterHI	=	0;
	pItem->Time				=	0x4104;
	pItem->Data				=	0x3908;
	pItem->StartClusterLO	=	0;
	pItem->FileLength		=	0;


	vCMD_pCommand->Command	=	cCMD_CMD_CREATE_FILE;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_CREATE_FILE_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_CREATE_FILE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_OpenFile
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	���ļ���
//
//��ڲ�����	pFileName	�ļ���������8���ֽ�
//				pExtName	��չ��������3���ֽ�
//
//����ֵ��		�Ƿ�򿪳ɹ���
//============================================================================================================
BOOL	CMD_OpenFile(	char*	pFileName,	char*	pExtName	)
{
	DWORD	i;

	//�����ļ�������չ��
	for	(i=0;	i<8;	i++)
		vCMD_pCommand->Parameter[i]	=	pFileName[i];
	for	(i=0;	i<3;	i++)
		vCMD_pCommand->Parameter[i+8]	=	pExtName[i];

	vCMD_pCommand->Command	=	cCMD_CMD_OPEN_FILE;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_OPEN_FILE_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_OPEN_FILE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_SetPointer
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	д�ļ���
//
//��ڲ�����	FilePointer	�ļ�ָ��
//
//����ֵ��		�Ƿ�д�ɹ���
//============================================================================================================
BOOL	CMD_SetPointer(	DWORD	FilePointer	)
{
	vCMD_pCommand->Parameter[1]	=	FilePointer;
	vCMD_pCommand->Parameter[2]	=	FilePointer>>8;
	vCMD_pCommand->Parameter[3]	=	FilePointer>>16;
	vCMD_pCommand->Parameter[4]	=	FilePointer>>24;

	vCMD_pCommand->Command	=	cCMD_CMD_SET_FILE_POINTER;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_SET_FILE_POINTER_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_SET_FILE_POINTER_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_Detect
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	��飬�Ƿ���U�̡�
//
//��ڲ�����	��
//
//����ֵ��		�Ƿ���U�̡�
//============================================================================================================
BOOL	CMD_Detect(	 void	)
{
	DWORD	i;

	vCMD_pCommand->Command	=	cCMD_CMD_DETECT_DEVICE;
	for	(i=0;i<63;i++)
	{
		vCMD_pCommand->Parameter[i]	=	i;
	}

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_DETECT_DEVICE_RSP))
	{	return	FALSE;	}
	else
	{
		//return	vCMD_pStatus->Result;
		/*if	(vCMD_pStatus->Result)
		{
			if	((vCMD_pStatus->Result)	!=	1)
			{
				vCMD_pStatus->Result	=	0;
			}
			//vCMD_pStatus->Result	=	1;
		}
		else
		{
			vCMD_pCommand->Parameter[0]	==	cCMD_CMD_DETECT_DEVICE_RSP;	
		}*/
		return	((vCMD_pStatus->Command	==	cCMD_CMD_DETECT_DEVICE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_GetVersion
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	�õ��汾�š�
//
//���ڲ�����	��
//
//����ֵ��		�Ƿ���U�̡�
//============================================================================================================
BOOL	CMD_GetVersion(	WORD*	pVersion	)
{

	vCMD_pCommand->Command	=	cCMD_CMD_GET_VERSION;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_GET_VERSION_RSP))
	{	return	FALSE;	}
	else
	{
		if	(((vCMD_pStatus->Command	==	cCMD_CMD_GET_VERSION_RSP)	&&	(vCMD_pStatus->Result)))
		{
			*pVersion	=	(vCMD_pStatus->Parameter[0])<<8	+	vCMD_pStatus->Parameter[1];
			return	TRUE;
		}
		else
		{	return	FALSE;	}
	}
}
//============================================================================================================
//��������		CMD_InDir
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	������Ŀ¼��
//
//��ڲ�����	pDirName	Ŀ¼��������8���ֽ�
//
//����ֵ��		�Ƿ�ɹ���
//============================================================================================================
BOOL	CMD_InDir(	char*	pDirName	)
{
	DWORD	i;

	//�����ļ�������չ��
	for	(i=0;	i<8;	i++)
		vCMD_pCommand->Parameter[i]	=	pDirName[i];

	vCMD_pCommand->Command	=	cCMD_CMD_IN_DIR;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_IN_DIR_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_IN_DIR_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_OutDir
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	�����ϼ�Ŀ¼��
//
//��ڲ�����	��
//
//����ֵ��		�Ƿ�ɹ���
//============================================================================================================
BOOL	CMD_OutDir(	 void	)
{

	vCMD_pCommand->Command	=	cCMD_CMD_OUT_DIR;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_OUT_DIR_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_OUT_DIR_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_RootDir
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	���ظ�Ŀ¼��
//
//��ڲ�����	��
//
//����ֵ��		�Ƿ�ɹ���
//============================================================================================================
BOOL	CMD_RootDir(	 void	)
{

	vCMD_pCommand->Command	=	cCMD_CMD_ROOT_DIR;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_ROOT_DIR_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_ROOT_DIR_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_MakeDir
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	����Ŀ¼��
//
//��ڲ�����	pDirName	Ŀ¼��������8���ֽ�
//
//����ֵ��		�Ƿ񴴽��ɹ���
//============================================================================================================
BOOL	CMD_MakeDir(	char*	pDirName	)
{
	PFS_DirItem	pItem;
	DWORD	i;

	pItem	=	(PFS_DirItem)&(vCMD_pCommand->Parameter[0]);

	//���
	for	(i=0;	i<11;	i++)
	{
		vCMD_pCommand->Parameter[i]	=	0x20;
	}
	for	(i=12;	i<63;	i++)
	{
		vCMD_pCommand->Parameter[i]	=	0;
	}

	//�����ļ�������չ��
	for	(i=0;	i<8;	i++)
	{
		if	(pDirName[i]	==	0)
			break;
		pItem->FileName[i]	=	pDirName[i];
	}
	pItem->Attribute		=	cFS_ATTR_DIRECTORY;
	pItem->ResForNT			=	0;
	pItem->CreateTimeTenth	=	88;
	pItem->CreateTime		=	0x4104;
	pItem->CreateDate		=	0x3908;
	pItem->LastAccDate		=	0x3908;
	pItem->StartClusterHI	=	0;
	pItem->Time				=	0x4104;
	pItem->Data				=	0x3908;
	pItem->StartClusterLO	=	0;
	pItem->FileLength		=	0;


	vCMD_pCommand->Command	=	cCMD_CMD_MAKE_DIR;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_MAKE_DIR_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_MAKE_DIR_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_Remove
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	ɾ���ļ���
//
//��ڲ�����	pFileName	�ļ���������8���ֽ�
//				pExtName	��չ��������3���ֽ�
//
//����ֵ��		�Ƿ�ɾ���ɹ���
//============================================================================================================
BOOL	CMD_Remove(	char*	pFileName,	char*	pExtName	)
{
	DWORD	i;

	//�����ļ�������չ��
	for	(i=0;	i<8;	i++)
		vCMD_pCommand->Parameter[i]	=	pFileName[i];
	for	(i=0;	i<3;	i++)
		vCMD_pCommand->Parameter[i+8]	=	pExtName[i];

	vCMD_pCommand->Command	=	cCMD_CMD_REMOVE_FILE;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_REMOVE_FILE_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_REMOVE_FILE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//��������		CMD_GetCapacity
//
//���ù�ϵ��	��	�ⲿ����	���á�
//
//����������	�õ�U��������ʣ��ռ䡣
//
//���ڲ�����	��
//
//����ֵ��		U��������ʣ��ռ䡣
//============================================================================================================
BOOL	CMD_GetCapacity(	DWORD*	AvailableSpace,	DWORD*	FreeSpace	)
{

	vCMD_pCommand->Command	=	cCMD_CMD_GET_CAPACITY;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_GET_CAPACITY_RSP))
	{	return	FALSE;	}
	else
	{
		if	(((vCMD_pStatus->Command	==	cCMD_CMD_GET_CAPACITY_RSP)	&&	(vCMD_pStatus->Result)))
		{
			*AvailableSpace	=	(vCMD_pStatus->Parameter[0])	+	(vCMD_pStatus->Parameter[1])<<8
							+	(vCMD_pStatus->Parameter[2])<<16	+	(vCMD_pStatus->Parameter[3])<<24;
			*FreeSpace		=	(vCMD_pStatus->Parameter[4])	+	(vCMD_pStatus->Parameter[5])<<8
							+	(vCMD_pStatus->Parameter[6])<<16	+	(vCMD_pStatus->Parameter[7])<<24;
			return	TRUE;
		}
		else
		{	return	FALSE;	}
	}
}
//============================================================================================================



//============================================================================================================
//
//			End	of	File
//
//============================================================================================================
