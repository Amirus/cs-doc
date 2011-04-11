// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "USB20C.H"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int gbool;
int gLong;
char str[10];
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_BUTTON2, m_Cmd2);
	DDX_Control(pDX, IDC_BUTTON1, m_Cmd1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnReadDMA)
	ON_BN_CLICKED(IDC_BUTTON2, OnOutput)
	ON_BN_CLICKED(IDC_BUTTON5, OnOpenInputState)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, TRUE);		// Set small icon
	
	// TODO: Add extra initialization here

   //===================================
   //��ʼ�������ϵͳ�Ƿ����豸����

   USB20C_DLLInit();    //�������USB20C.DLL��̬���ӿ�

   //-------------------------------
   //�ж�ϵͳ�������ж��ٿ�USB20Cģ��
   //gLong�ķ���ֵ��ʾϵͳ���ж���(USB)�豸������.

    gLong=USB20C_EnumDeviceCount();
	itoa(gLong,str,10);
    m_List.AddString(str);
   //--------------------------------

   gLong=USB20C_Init(1,1);	
   if (gLong==0)   //��ϵͳû�������κ�USBʱ����0
   {
	   gLong=USB20C_GetLastError(); //����USB���������,���κγ���ʱ�����Ե������Է��ص��ú����Ĵ���״̬
       MessageBox("δ�����κ� USB20C �豸��","USB20C.DLL DEMO");
	   return 0;
   }

   //-------------------------------   
   //�ж�USB20C�����ں���ģʽ
   gbool = USB20C_WorkAtHighSpeed();
   if (gbool)
   {
      MessageBox("USB20C�����ڸ���ģʽ","USB20C.DLL DEMO");
   }
   else
   {
      MessageBox("USB20C������ȫ��ģʽ","USB20C.DLL DEMO");
   }

   //====================================
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnCancel() 
{
	  //�ر��豸���رմ���

	  USB20C_Done();       //�ر��豸

	  USB20C_DLLDone();    //�ͷŶ�̬��(.DLL)

	  CDialog::OnCancel();
}

void CTestDlg::OnOK() 
{
		
    //���ϵͳ���ӵ��豸��Ŀ
    
    
	//gLong�ķ���ֵ��ʾϵͳ���ж���(USB)�豸������.
	//itoa(gLong,str,10);
    //m_List.AddString(str);
     
    //========================================
	//ģ���϶����ݺ��� USB20C_MultInput(Num,viod *Data)
	
    short ArrData[10];
	int i;

    for (i=0;i<10;i++)
	{
      ArrData[i] = i;
	}
	gbool = USB20C_MultInput(10, &ArrData[0]);   //Num��60
    
	if (gbool)
	{
		m_List.AddString("Ture");
		for (i=0;i<10;i++)
		{
         itoa(ArrData[i],str,10);
	     m_List.AddString(str);
		}
	}
	else
	{
        MessageBox("Error","Caption");
	    m_List.AddString("False");
	}

}

void CTestDlg::OnReadDMA() 
{
	
	/**���Զ�DMA
	// 1: ���ۺ�ʱ�ڵ���DMA��ʽǰ�����ȵ��� USB20C_Init()������ʼ������.
	// 2: ���ùر�(DMA) �� USB20C_EndDMA() ����.
	// 3: ���趨��ַ�����ض��ĵ�ַ�� USB20C_MultOutput(3,&buf[0])
	// 4: ���� USB20C_StartDMARead() ����׼����ȡ����
	// 5: ���� USB20C_DMARead ������ʼ��ȡ����
	// 6: ���ݶ�ȡ��Ϻ�ر�(DMA) USB20C_EndDMA()
	/*/
	short buff[10];
	int getLong[1];
	byte buf[6];
	buf[0]=0xB;
    buf[1]=0;
    buf[2]=0xF;
    buf[3]=4;
    buf[4]=0xB;
    buf[5]=1;

	int sTime=2000;
	
	m_List.ResetContent(); 

	USB20C_EndDMA();
	USB20C_MultOutput(3,&buf[0]);

    gbool=USB20C_StartDMARead();
    if (gbool)
	{
       gbool=USB20C_DMARead(&buff[0],20,&getLong[0],sTime);
	   if (gbool)
	   {
		  MessageBox("DMARead =True","DMARead Caption");
		  int j;
		  m_List.AddString("------------------------------");
		  for (j=0;j<10;j++)
		  {
            itoa(buff[j],str,10);
			m_List.AddString(str);
		  }
	   }
	   else
	   {
		   MessageBox("DMARead =False","Caption");
		   USB20C_Done();
	   }
    }
	else
	{
		MessageBox("Start_DMARead =False","Caption");
		USB20C_Done();
	}
	USB20C_EndDMA();
}

void CTestDlg::OnOutput() 
{
	
	int i;
	char j[20];
	i=0;
    m_Edit.GetWindowText(j,m_Edit.GetLimitText());
	gbool=USB20C_Output(i, atoi(j));

}

void CTestDlg::OnOpenInputState() 
{
	//�����״̬

	USB20C_Output(2, 3);
	m_Cmd2.EnableWindow(true);
}
/***
void CTestDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	char str[10]={49,50,51,51};
	char size[10];
	itoa(str[1],size,16);
    //m_List.AddStrim(size);
}
/*/
