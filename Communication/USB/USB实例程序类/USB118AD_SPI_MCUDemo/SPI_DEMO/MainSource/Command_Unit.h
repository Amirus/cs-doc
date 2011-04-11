//============================================================================================================
//
//		ͷ�ļ���		Command_Unit.h
//
//============================================================================================================

//============================================================================================================
#ifndef __My_Command_Func_H_
#define __My_Command_Func_H_
//============================================================================================================



//============================================================================================================
//	����ͷ�ļ�
//============================================================================================================


//============================================================================================================
//
//		����	����	��		����
//
//============================================================================================================
//*******************************
//����Ĺ���״̬
//*******************************
#define	cCMD_WorkStatus_SearchUDisk	0x0000
#define	cCMD_WorkStatus_SearchSig	0x0001
#define	cCMD_WorkStatus_WaitCmd		0x0002
#define	cCMD_WorkStatus_WaitCmdData	0x0004
#define	cCMD_WorkStatus_DoCmd		0x0008
#define	cCMD_WorkStatus_SendStatus	0x0010
#define	cCMD_EndEvent_ReadWrite		0x8000
//*******************************
//������ͨѶ�������״̬����
//*******************************
#define cCMD_CMD_RESET				0x00
/////////////////////////////////////
#define cCMD_CMD_DETECT_DEVICE		0x01
#define cCMD_CMD_DETECT_DEVICE_RSP	0x04
/////////////////////////////////////
#define cCMD_CMD_MAKE_DIR			0x32
#define cCMD_CMD_IN_DIR				0x33
#define cCMD_CMD_OUT_DIR			0x34
#define cCMD_CMD_ROOT_DIR			0x35
//----------
#define cCMD_CMD_MAKE_DIR_RSP		0x42
#define cCMD_CMD_IN_DIR_RSP			0x43
#define cCMD_CMD_OUT_DIR_RSP		0x44
#define cCMD_CMD_ROOT_DIR_RSP		0x45
/////////////////////////////////////									
#define cCMD_CMD_OPEN_FILE			0x06
#define cCMD_CMD_CREATE_FILE		0x07
#define cCMD_CMD_READ_FILE			0x08
#define cCMD_CMD_WRITE_FILE			0x09
//----------
#define cCMD_CMD_CREATE_FILE_RSP	0x0A
#define cCMD_CMD_READ_FILE_RSP		0x0B
#define cCMD_CMD_WRITE_FILE_RSP		0x0C
#define cCMD_CMD_OPEN_FILE_RSP		0x0D
/////////////////////////////////////
#define cCMD_CMD_LIST				0x10
#define cCMD_CMD_REMOVE_FILE		0x11
#define cCMD_CMD_GET_CAPACITY		0x12
#define cCMD_CMD_GET_FREE_CAPACITY	0x13
#define cCMD_CMD_FORMAT_DISK		0x14
#define cCMD_CMD_SET_FILE_POINTER	0x15
//-----------
#define cCMD_CMD_LIST_RSP			0x20
#define cCMD_CMD_REMOVE_FILE_RSP	0x21
#define cCMD_CMD_GET_CAPACITY_RSP	0x22
#define cCMD_CMD_GET_FREE_CAPACITY_RSP	0x23
#define cCMD_CMD_FORMAT_DISK_RSP		0x24
#define cCMD_CMD_SET_FILE_POINTER_RSP	0x25
/////////////////////////////////////
#define cCMD_CMD_GET_VERSION		0x30
#define cCMD_CMD_GET_VERSION_RSP	0x40
/////////////////////////////////////
#define cCMD_CMD_GET_ReadSector		0x50
#define cCMD_CMD_GET_ReadSector_RSP	0x51
/////////////////////////////////////
#define cCMD_CMD_DIR_DOWN			0x70
#define cCMD_CMD_DIR_DOWN_RSP		0x80
#define cCMD_CMD_DIR_UP				0x71
#define cCMD_CMD_DIR_UP_RSP			0x81		
///////////////////////////////////////////////
#define cCMD_ERC_OK				0x0000
/////////////////////////////////////
#define cCMD_ERC_NODEVICE		0x0001
#define cCMD_ERC_DEVICEFULL		0x0002
#define cCMD_ERC_DEVICEERR		0x000a
#define cCMD_ERC_FILEEXIST		0x0010
#define cCMD_ERC_FILENOTFOUND	0x0011
#define cCMD_ERC_LENGTHEXCEED	0x0012
#define cCMD_ERC_REACHEND		0x0013
#define cCMD_ERC_FILENOTOPENED	0x0014
#define cCMD_ERC_DIRNOTEMPTY	0x0015
#define cCMD_ERC_STATEERR		0x0050
#define cCMD_ERC_SYSERR			0x00fa
/////////////////////////////////////
#define cCMD_COMERC_CMDERR		0x01
#define cCMD_COMERC_TIMEOUT		0x02
//============================================================================================================

//============================================================================================================
//
//		����	��������	�Ľṹ��
//
//============================================================================================================
//========================================================
//*******************************
//ReadSector�ṹ��
//*******************************
typedef	__packed	struct	tCMD_ReadSector_CMD
{
	BYTE	Res1;
	DWORD	SectorAddr;
	BYTE	Res[58];
} TCMD_ReadSector_CMD, *PCMD_ReadSector_CMD;
//*******************************
//Write�ṹ��
//*******************************
typedef	__packed	struct	tCMD_Write_CMD
{
	BYTE	Res1;
	WORD	DataLength;
	BYTE	Res[60];
} TCMD_Write_CMD, *PCMD_Write_CMD;
//---
typedef	__packed	struct	tCMD_Write_RSP
{
	BYTE	Res[60];
} TCMD_Write_RSP, *PCMD_Write_RSP;
//*******************************
//List�ṹ��
//*******************************
typedef	__packed	struct	tCMD_List_RSP
{
	WORD	DataLength;
	BYTE	Res[58];
} TCMD_List_RSP, *PCMD_List_RSP;
//*******************************
//InDir�ṹ��
//*******************************
typedef	__packed	struct	tCMD_InDir_CMD
{
	char	Name[8];
	BYTE	Res[55];
} TCMD_InDir_CMD, *PCMD_InDir_CMD;
//*******************************
//OpenFile�ṹ��
//*******************************
typedef	__packed	struct	tCMD_OpenFile_CMD
{
	char	FileName[8];
	char	ExtName[3];
} TCMD_OpenFile_CMD, *PCMD_OpenFile_CMD;
//*******************************
//SetPointer�ṹ��
//*******************************
typedef	__packed	struct	tCMD_SetPointer_CMD
{
	BYTE	Res;
	DWORD	NewFilePointer;
} TCMD_SetPointer_CMD, *PCMD_SetPointer_CMD;
//*******************************
//�ṹ��
//*******************************
typedef	__packed	struct	tCMD__CMD
{
	BYTE	Res[63];
} TCMD__CMD, *PCMD__CMD;
//---
typedef	__packed	struct	tCMD__RSP
{
	BYTE	Res[60];
} TCMD__RSP, *PCMD__RSP;
//============================================================================================================


//============================================================================================================
//
//		����	���պͷ���	������ṹ��
//
//============================================================================================================
//*******************************
//һЩ����
//*******************************
#define	cCMD_Signature	0xBBAA
#define	cCMD_Signature0	0xAA
#define	cCMD_Signature1	0xBB
#define	cCMD_MAXOUTDATA	4096
#define	cCMD_MAXINDATA	4096
//========================================================
//*******************************
//�������ݽṹ��
//*******************************
typedef	__packed	struct	tCMD_CommandBlock
{
	WORD	Signature;			//Ӧ����һ������	cCMD_Signature
	BYTE	Command;			//�������
	BYTE	Parameter[63];		//����Ĳ���
	BYTE	Data[cCMD_MAXOUTDATA];			//����Я��������
} TCMD_CommandBlock, *PCMD_CommandBlock;
//*******************************
//�������ݽṹ��
//*******************************
typedef	__packed	struct	tCMD_StatusBlock
{
//	WORD	Signature;			//Ӧ����һ������	cCMD_Signature
	BYTE	Command;			//Ӧ�����
	BYTE	Result;				//Ӧ����
	WORD	ErrorCode;			//�������
	BYTE	Parameter[60];		//Ӧ��Ĳ���
	BYTE	Data[cCMD_MAXINDATA];		//Ӧ��Я��������
} TCMD_StatusBlock, *PCMD_StatusBlock;
//============================================================================================================


//============================================================================================================
//****************************************
//����	�ļ�Ŀ¼�����ļ���8.3��ʽ��		������
//****************************************
#define	cFS_ATTR_READ_ONLY 0x01
#define	cFS_ATTR_HIDDEN 0x02
#define	cFS_ATTR_SYSTEM 0x04
#define	cFS_ATTR_VOLUME_ID 0x08
#define	cFS_ATTR_DIRECTORY 0x10
#define	cFS_ATTR_ARCHIVE 0x20
#define	cFS_ATTR_LONG_NAME	(cFS_ATTR_READ_ONLY |	cFS_ATTR_HIDDEN |	cFS_ATTR_SYSTEM |	cFS_ATTR_VOLUME_ID)
#define	cFS_ATTR_LONG_NAME_MASK	(cFS_ATTR_LONG_NAME	|	cFS_ATTR_DIRECTORY	|	cFS_ATTR_ARCHIVE)
//****************************************
//����	�ļ�Ŀ¼�����ļ���8.3��ʽ��		�Ľṹ��
//****************************************
typedef	__packed	struct tFS_DirItem
{
	char	FileName[8];	//ofs:0.�ļ���
	char	ExtName[3];		//ofs:8.��չ��
	BYTE	Attribute;		//ofs:11.�ļ����ԡ�����ֵ���浵(0x20)�����(0x08)��
	BYTE	ResForNT;		//ofs:12.WinNT����
	BYTE	CreateTimeTenth;//ofs.13.
	WORD	CreateTime;		//ofs.14.����ʱ��
	WORD	CreateDate;		//ofs.16.��������
	WORD	LastAccDate;	//ofs.18.����������
	WORD	StartClusterHI;	//ofs.20.��ʼ�غţ���16λ��
	WORD	Time;			//ofs:22.ʱ��
	WORD	Data;			//ofs:24.����
	WORD	StartClusterLO;	//ofs:26.��ʼ�غţ���16λ��
	DWORD	FileLength;		//ofs:28.�ļ�����
}	TFS_DirItem,	*PFS_DirItem; 
//============================================================================================================



//============================================================================================================
//
//		����	����ĺ���
//
//============================================================================================================
//�����
void	CMD_Init(DWORD*	WriteBuffer,	DWORD*	ReadBuffer);
BOOL	CMD_WriteFile(	DWORD	DataLength	);
BOOL	CMD_ReadFile(	DWORD*	DataLength	);
BOOL	CMD_CreateFile(	char*	pFileName,	char*	pExtName	);
BOOL	CMD_OpenFile(	char*	pFileName,	char*	pExtName	);
BOOL	CMD_SetPointer(	DWORD	FilePointer	);
BOOL	CMD_Detect(	 void	);
BOOL	CMD_GetVersion(	WORD*	pVersion	);
BOOL	CMD_InDir(	char*	pDirName	);
BOOL	CMD_OutDir(	 void	);
BOOL	CMD_RootDir(	 void	);
BOOL	CMD_MakeDir(	char*	pDirName	);
BOOL	CMD_Remove(	char*	pFileName,	char*	pExtName	);
BOOL	CMD_GetCapacity(	DWORD*	AvailableSpace,	DWORD*	FreeSpace	);

//============================================================================================================




//============================================================================================================
#endif  // __My_Command_Func_H_
//============================================================================================================
//
//			End	of	File
//
//============================================================================================================
