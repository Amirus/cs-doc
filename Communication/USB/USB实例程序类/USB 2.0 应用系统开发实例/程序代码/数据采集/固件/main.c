#include "config.h"


unsigned char Device_Descriptor[18] = {0x12,0x01, 0x10, 0x01,0xDC,0x0,0x0,0x20,       
						               0x05, 0x80,0x00, 0x10,0x00,0x00,0,0,0,0x01          	
						              };
unsigned char Configuration_Descriptor_All[32] = {
		        	    9,2,0x20,0x00,1,1,0,0x80,0xfa,//����������
                        9,4,0,0,2,0xDC,0xAC,0xBC,0, //�ӿ�������                  
					    7,5,0x81,0x02,0x20,0x00,0x0,//�˿�������            
						7,5,0x02,0x02,0x20,0x00,0x0  //�˿�������         
				     	};
bit caiji_start,usb_connected,usb_connected_stored,usb_configured_stored;
int bufout[256],gaptime,gl_reminder_inc_ms,i,j;
unsigned char   bmRequestType,usb_sof_counter,usb_configuration_nb,*pbuffer,endpoint_status[2];
////////////////////////////
void main (void)//������
{
  usb_task_init();//USB��ʼ��
  EpEnable();//�˿�ʹ��
  while(1)
  {  
  usb_task();//USB������ 
  for(i=0;i<100;i++);
  caiji();//���ݲɼ�������
  }

}
////////////////////////////
void usb_task_init(void)
{ 
  USBCON |= 0x80; //ʹ��USB������
  USBCON |= 0x10; /*USB������*/
  delay(100);
  USBCON &= ~0x10;
  PLLDIV = 32; //���ÿ�����ʱ��
  PLLCON |= 0x02;//ʹ��PLL
  UEPNUM = 0;//��λ���ƶ˿�
  UEPCONX = 0;
  /*��ʼ��״̬����*/
  usb_connected = 0;
  usb_connected_stored = 1;
  usb_configured_stored = 0;
  endpoint_status[0] = 0x00;
  endpoint_status[1] = 0x00;
  usb_connected = 0;
  usb_configuration_nb = 0x00;
}
///////////////////////////////////
//////////////////////////////////////
void EpEnable(void)
{
	UEPNUM=0x00;	UEPCONX=0x80;//�˿�0
	UEPNUM=0x01;	UEPCONX=0x86;//�˿�1
	UEPNUM=0x02;	UEPCONX=0x82;//�˿�2
	UEPRST=0x07;	UEPRST= 0x00;//�˿ڸ�λ
	UEPIEN=0x07;	USBIEN|=0x01<<4;
	USBADDR=0x01<<7;
}
////////////////////////////////////////////////
void usb_task(void){
      if (UEPINT & EP0)usb_enumeration_process();
      if (UEPINT & EP1)   
	    {
	     Usb_clear_tx_complete();	//UEPINT��ַ0xF8ȴ����λѰַ	
	    }
	   if (UEPINT & EP2)   
		 {              
         unsigned char  Bufin[64];  
         i = ReadEp(2,Bufin);
		 if(Bufin[1]&0x80!=0)
		  {
          caiji_start=1;//���òɼ���־λ
          gaptime= Bufin[0];//�ɼ�Ƶ��	
          if(Bufin[1]&0x7f==1)
            {
             P1_4=1;
             }
          if(Bufin[1]&0x7f==2)
             {
             P1_5=1;
             }
          if(Bufin[1]&0x7f==3)
             {
             P1_6=1;
             }
           if(Bufin[1]&0x7f==4)
             {
             P1_7=1;
             }                			
		   }
           else
           {
             caiji_start=0;//���òɼ�״̬λ
           }
    /*���ݻ���������*/             
	for(i=0;i<64;i++)
		{
		Bufin[i]=0;
		}
   }           
  }
/////////////////////////////////////////////////////////
////////////////////////////////////////////////
void usb_enumeration_process (void)//USB�豸ö�ٹ���
{ 
  UEPNUM = 0;//ѡ����ƶ˿�
  bmRequestType = UEPDATX;          /* �õ� bmRequestType */
  switch (UEPDATX)                  /* �ж� bRequest ��ֵ */
  {
    case GET_DESCRIPTOR:                   /*�������������*/
      usb_get_descriptor();
      break;
    case GET_CONFIGURATION:               /*�����������*/
      usb_get_configuration();
      break;
    case SET_ADDRESS:                    /*���õ�ַ����*/
      usb_set_address();
      break;
    case SET_CONFIGURATION:              /*�����������������HID��������*/
      if (bmRequestType == 0) { usb_set_configuration(); }
         break;
     default:
      UEPSTAX =UEPSTAX & ~0x04;        /*���SETUP��־*/
      UEPSTAX =UEPSTAX | 0x20;         /*��ֹ��������*/               
      while (!UEPSTAX & 0x08);
      UEPSTAX =UEPSTAX &~ 0x20;
      UEPSTAX =UEPSTAX &~ 0x08;
      break;
    }
    UEPSTAX = UEPSTAX &~0x80;
}
//////////////////////////////////////////////////////
void usb_set_address (void)                 /*�����豸��ַ�ӳ���*/
{
  unsigned char address;
  address = UEPDATX;                    /* ���ϵͳ������豸��ַ */
  UEPSTAX &= ~0x04;                     /*���SETUP��־*/
  UEPSTAX |= 0x10;                       /* ����0�ֽ�״̬�� */
  USBCON |= 0x01;                         /*���õ�ַʹ��*/
  while (!(UEPSTAX & 0x01));
  UEPSTAX=UEPSTAX & ~0x01;
  USBADDR = (0x80 | address);              /*�����豸��ַ*/
}
////------set_config-------////////
void usb_set_configuration (void)
{
  unsigned char configuration_number;
  configuration_number = UEPDATX;   /* ��ȡ������*/
  UEPSTAX &= ~0x80;
  UEPSTAX &= ~0x04;                   /*���SETUP��־*/
  if (configuration_number <= 1)
  {
    usb_configuration_nb = configuration_number;
  }
  else
  {
    UEPSTAX |= 0x20;            /*��ֹ��������*/ 
    while (!UEPSTAX & 0x08);
    UEPSTAX &= ~0x20;
    UEPSTAX &= ~0x08;
    return;
  }

  UEPSTAX |= 0x10;        /* ����0�ֽ�״̬��*/
  while (!UEPSTAX & 0x01);
  UEPSTAX &= ~0x01;
  /* �豸�˿����� */
  UEPNUM = 1;
  UEPCONX = 0x87;
  UEPRST = 0x01;
  UEPRST = 0x00;
}
///////////////////////////////////////////////////////////////
void usb_get_descriptor (void)
{
  unsigned char   data_to_transfer;
  unsigned  int  wLength;
  unsigned char   descriptor_type;
  unsigned char   string_type;                      
  string_type = UEPDATX;            /* ��ȡ wValue�ĵ�λ */
  descriptor_type = UEPDATX;        /* ��ȡ wValue�ĸ�λ */
  switch (descriptor_type)          /*�ж�����������*/
  {
    case DEVICE:                            /*�豸�������*/                
    {
      data_to_transfer = sizeof (Device_Descriptor);
      pbuffer = &(Device_Descriptor[0]);//ָ���豸�������ṹ�׵�ַ
      break;
    }

    case CONFIGURATION:                       /*����*/
    {
      data_to_transfer = sizeof (Configuration_Descriptor_All);
      pbuffer = &(Configuration_Descriptor_All[0]);
      break;
   } 
    default:
    {
      UEPSTAX &= ~0x04;
      UEPSTAX |= 0x20;
      while ((!(UEPSTAX & 0x08)) && (UEPSTAX & 0x04));
      UEPSTAX &= ~0x08;
      UEPSTAX &= ~0x20;
      UEPSTAX &= ~0x80;
      return;
    }
  }

  ACC = UEPDATX;                   
  ACC = UEPDATX;
  ((unsigned char*)&wLength)[1] = UEPDATX;   /* ��ȡҪ����ĳ��� */
  ((unsigned char*)&wLength)[0] = UEPDATX;
  if (wLength > data_to_transfer);     /* ��ȡ�ĳ��ȴ�������������ʱ */      
  else
  {
    data_to_transfer = (unsigned char)wLength;       /*������Ҫ�����ݳ��� */
  }
  UEPSTAX &= ~0x04 ;                    
  UEPSTAX |= 0x80;                            
  while (data_to_transfer > 32)/*���͵ĳ��ȴ��ڿ��ƶ˿ڵĳ���ʱ*/
  {
    pbuffer = usb_send_ep0_packet(pbuffer, 32);/*����һ�ζ˿ڳ��ȵ�����*/
    data_to_transfer -= 32;
    while ((!(UEPSTAX & 0x42)) && (!(UEPSTAX & 0x01)));/*����û�����*/
    UEPSTAX=UEPSTAX & 0x01;
    if ((UEPSTAX & 0x42))               
    {
      UEPSTAX &= ~0x10;
      UEPSTAX &= ~0x02;
      return;
    }
  }
  /* �������һ������ */
  pbuffer = usb_send_ep0_packet(pbuffer, data_to_transfer);
  data_to_transfer = 0;
  while ((!(UEPSTAX & 0x42)) && (!(UEPSTAX & 0x01)));
  UEPSTAX=UEPSTAX & 0x01;
  if ((UEPSTAX & 0x42))                  /* if cancel from USB Host */
  {
    UEPSTAX &= ~0x10;
    UEPSTAX &= ~0x02;
    return;
  }
}
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
unsigned int caiji_single()
{//��������
   unsigned int dd;
   P1_0=0;
   P1_1=0;
   for(i=0;i<10;i++);
   P1_0=1;
   P1_1=1;
  //��ѯת���Ƿ����
  if(P1_3==1);
   //��ȡת������
  P1_0=0;
  P1_2=0;
  dd=P1;
  return dd;
}
//////////////////////////////////////////////////////
void caiji()
{
if(caiji_start == 1)
{
for(i=0;i<256;i++)
{
bufout[i]=caiji_single();
delay(gaptime);
}
for(i=0;i<100;i++);
senddata();
}
}
/////////////////////////////////

void senddata()
{

for(i=0;i<8;i++)
{
 for(j=0;j<32;j++)
{
  Usb_write_byte(bufout[i*32+j]);
}
Usb_set_tx_ready();
while(!Usb_tx_complete());
Usb_clear_tx_complete();
}
}
/////////////////////////////

void delay(int mm)
{
for(i=0;i<mm;i++);
}
