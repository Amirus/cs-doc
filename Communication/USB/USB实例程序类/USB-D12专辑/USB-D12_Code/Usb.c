/******************************************************************
   ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
      �ҵ����䣺computer-lov@tom.com
        ��ӭ�����ҵ�blog��  http://computer00.21ic.org

USB.C  file

Created by Computer-lov
Date: 2004.9.21

Edit date:2006.3.2

Version V1.1
Copyright(C) Computer-lov 2004-2014
All rigths reserved
             
*******************************************************************/

#include<Atmel/AT89x52.H>
#include<USB.H>
#include<PDIUSBD12.H>
#include<MASS_STORAGE.H>
//#include<serial.h>


#define USB_COMMAND_ADD           1
#define USB_DATA_ADD              0

#define USB_DATA                  P0
#define USB_A0                    P3_5
#define USB_WR                    P3_6
#define USB_RD                    P3_7
#define USB_INT                   P3_2


#define ENDPOINT_NUMBER           2
#define MAX_CONTROL_DATA_SIZE     16
#define MAX_LUN                    0
#define MAX_BULK_DATA_SIZE        64

CONTROL_DATA_BUFF Control_Data;

unsigned char idata buffer[64];

CBW cbw;
CSW csw;


union 
{
 unsigned char Register;
 struct
  {
   unsigned char Command                :1;
   unsigned char Data                   :1;
  }Status;
}SCSI;

unsigned char *pData;

code unsigned char DISK_INF[36]=                                      //������Ϣ
{
 0x00,
 0x00,
 0x02,
 0x02,
 0x1F,
 0x00,0x00,0x00,
 0xB5,0xE7,0XC4,0xD4,0xC8,0xA6,0xC8,0xA6, 
 0xD7,0xD4,0xBC,0xBA,0xD7,0xF6,0xB5,0xC4,0xBC,0xD9,0x55,0xC5,0xCC,0x00,0x00,0x00,
 0x31,0x2E,0x30,0x31
};

code unsigned char DISK_CAPACITY[8]=                                  //��������
{
 0x00,0x01,0xff,0x00,
 0x00,0x00,0x02,0x00
};

code unsigned char SENSE[0x12]=                                      //ģʽ̽�ⷵ������
{
0x70, 0x00, 0x05, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00
};

code unsigned char DBR[512]=                                          //DOS������¼
{
 0xeb, 0x3e, 0x90,                                                    //��תָ��
 0x4d, 0x53, 0x44, 0x4f, 0x53, 0x35, 0x2e, 0x30,                      //�ļ�ϵͳ�汾��Ϣ
 0x00, 0x02,                                                          //�����ֽ���
 0x20,                                                                //ÿ��������
 0x01, 0x00,                                                          //����������
 0x02,                                                                //�÷�����FAT������
 0xF0, 0x01,                                                          //��Ŀ¼����
 0x00, 0x00,                                                          //С������
 0xf8,                                                                //ý��������
 0x10, 0x00,                                                          //ÿFAT������
 0x20, 0x00,                                                          //ÿ��������
 0x40, 0x00,                                                          //��ͷ��
 0x00, 0x00, 0x00, 0x00,                                              //����������
 0x40, 0xff, 0x01, 0x00,                                              //��������
 0x80,                                                                //����������������80��ʾӲ��
 0x00,                                                                //����
 0x29,                                                                //��չ������ǣ�0x29��ʾ������������ 
 0x88, 0x09, 0x71, 0x20,                                              //������к�
 0xBC, 0xD9, 0x55, 0xC5, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    //���̱��
 0x46, 0x41, 0x54, 0x31, 0x36, 0x20,  0x20, 0x20,                     //�ļ�ϵͳ������Ϣ
 0xf1, 0x7d,
 0xfa, 0x33, 0xc9, 0x8e,  0xd1, 0xbc, 0xfc, 0x7b,  0x16, 0x07, 0xbd, 0x78,  0x00, 0xc5, 0x76, 0x00,
 0x1e, 0x56, 0x16, 0x55,  0xbf, 0x22, 0x05, 0x89,  0x7e, 0x00, 0x89, 0x4e,  0x02, 0xb1, 0x0b, 0xfc,
 0xf3, 0xa4, 0x06, 0x1f,  0xbd, 0x00, 0x7c, 0xc6,  0x45, 0xfe, 0x0f, 0x8b,  0x46, 0x18, 0x88, 0x45,
 0xf9, 0xfb, 0x38, 0x66,  0x24, 0x7c, 0x04, 0xcd,  0x13, 0x72, 0x3c, 0x8a,  0x46, 0x10, 0x98, 0xf7,
 0x66, 0x16, 0x03, 0x46,  0x1c, 0x13, 0x56, 0x1e,  0x03, 0x46, 0x0e, 0x13,  0xd1, 0x50, 0x52, 0x89,
 0x46, 0xfc, 0x89, 0x56,  0xfe, 0xb8, 0x20, 0x00,  0x8b, 0x76, 0x11, 0xf7,  0xe6, 0x8b, 0x5e, 0x0b,
 0x03, 0xc3, 0x48, 0xf7,  0xf3, 0x01, 0x46, 0xfc,  0x11, 0x4e, 0xfe, 0x5a,  0x58, 0xbb, 0x00, 0x07,
 0x8b, 0xfb, 0xb1, 0x01,  0xe8, 0x94, 0x00, 0x72,  0x47, 0x38, 0x2d, 0x74,  0x19, 0xb1, 0x0b, 0x56,
 0x8b, 0x76, 0x3e, 0xf3,  0xa6, 0x5e, 0x74, 0x4a,  0x4e, 0x74, 0x0b, 0x03,  0xf9, 0x83, 0xc7, 0x15,
 0x3b, 0xfb, 0x72, 0xe5,  0xeb, 0xd7, 0x2b, 0xc9,  0xb8, 0xd8, 0x7d, 0x87,  0x46, 0x3e, 0x3c, 0xd8,
 0x75, 0x99, 0xbe, 0x80,  0x7d, 0xac, 0x98, 0x03,  0xf0, 0xac, 0x84, 0xc0,  0x74, 0x17, 0x3c, 0xff,
 0x74, 0x09, 0xb4, 0x0e,  0xbb, 0x07, 0x00, 0xcd,  0x10, 0xeb, 0xee, 0xbe,  0x83, 0x7d, 0xeb, 0xe5,
 0xbe, 0x81, 0x7d, 0xeb,  0xe0, 0x33, 0xc0, 0xcd,  0x16, 0x5e, 0x1f, 0x8f,  0x04, 0x8f, 0x44, 0x02,
 0xcd, 0x19, 0xbe, 0x82,  0x7d, 0x8b, 0x7d, 0x0f,  0x83, 0xff, 0x02, 0x72,  0xc8, 0x8b, 0xc7, 0x48,
 0x48, 0x8a, 0x4e, 0x0d,  0xf7, 0xe1, 0x03, 0x46,  0xfc, 0x13, 0x56, 0xfe,  0xbb, 0x00, 0x07, 0x53,
 0xb1, 0x04, 0xe8, 0x16,  0x00, 0x5b, 0x72, 0xc8,  0x81, 0x3f, 0x4d, 0x5a,  0x75, 0xa7, 0x81, 0xbf,
 0x00, 0x02, 0x42, 0x4a,  0x75, 0x9f, 0xea, 0x00,  0x02, 0x70, 0x00, 0x50,  0x52, 0x51, 0x91, 0x92,
 0x33, 0xd2, 0xf7, 0x76,  0x18, 0x91, 0xf7, 0x76,  0x18, 0x42, 0x87, 0xca,  0xf7, 0x76, 0x1a, 0x8a,
 0xf2, 0x8a, 0x56, 0x24,  0x8a, 0xe8, 0xd0, 0xcc,  0xd0, 0xcc, 0x0a, 0xcc,  0xb8, 0x01, 0x02, 0xcd,
 0x13, 0x59, 0x5a, 0x58,  0x72, 0x09, 0x40, 0x75,  0x01, 0x42, 0x03, 0x5e,  0x0b, 0xe2, 0xcc, 0xc3,
 0x03, 0x18, 0x01, 0x27,  0x0d, 0x0a, 0x49, 0x6e,  0x76, 0x61, 0x6c, 0x69,  0x64, 0x20, 0x73, 0x79,
 0x73, 0x74, 0x65, 0x6d,  0x20, 0x64, 0x69, 0x73,  0x6b, 0xff, 0x0d, 0x0a,  0x44, 0x69, 0x73, 0x6b,
 0x20, 0x49, 0x2f, 0x4f,  0x20, 0x65, 0x72, 0x72,  0x6f, 0x72, 0xff, 0x0d,  0x0a, 0x52, 0x65, 0x70,
 0x6c, 0x61, 0x63, 0x65,  0x20, 0x74, 0x68, 0x65,  0x20, 0x64, 0x69, 0x73,  0x6b, 0x2c, 0x20, 0x61,
 0x6e, 0x64, 0x20, 0x74,  0x68, 0x65, 0x6e, 0x20,  0x70, 0x72, 0x65, 0x73,  0x73, 0x20, 0x61, 0x6e,
 0x79, 0x20, 0x6b, 0x65,  0x79, 0x0d, 0x0a, 0x00,  0x49, 0x4f, 0x20, 0x20,  0x20, 0x20, 0x20, 0x20,
 0x53, 0x59, 0x53, 0x4d,  0x53, 0x44, 0x4f, 0x53,  0x20, 0x20, 0x20, 0x53,  0x59, 0x53, 0x80, 0x01,
 0x00, 0x57, 0x49, 0x4e,  0x42, 0x4f, 0x4f, 0x54,  0x20, 0x53, 0x59, 0x53,  0x00, 0x00, 0x55, 0xaa,
};

code unsigned char FAT[64]=                                             //ģ����ļ������
{
 0xF8, 0xFF, 0xFF, 0xFF,  0xff, 0xff, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00 ,0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00 ,0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00 ,0x00, 0x00,  0x00, 0x00, 0x00, 0x00
};

code unsigned char ZERO[64]=                                            //���0
{
 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00 ,0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00 ,0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00 ,0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00 ,0x00, 0x00,  0x00, 0x00, 0x00, 0x00
};

code unsigned char ROOT_DIR[64]=                                       //��Ŀ¼
{
 0xBC, 0xD9, 0x55, 0xC5, 0xCC, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,   //���̱��
 0x08,                                                               //�ļ����ԣ���ʾ���̱��
 0x00,                                                               //����
 0x00,                                                               //����ʱ�����ʱ���
 0x00, 0x40,                                                         //�ļ�����ʱ��
 0x41, 0x31,                                                         //�ļ���������
 0x42, 0x31,                                                         //����������
 0x00, 0x00,                                                         //��ʼ�غŸ�λ�ֽڣ�FAT12/16����Ϊ0
 0x00, 0x40,                                                         //���дʱ��
 0x42, 0x31,                                                         //���д����
 0x00, 0x00,                                                         //��ʼ�ص�λ�ֽ� 
 0x00, 0x00, 0x00, 0x00,                                             //�ļ�����
 'T',  'E',   'S',  'T', ' ', ' ', ' ', ' ',  'T', 'X', 'T',         //�ļ���
 0x01,                                                               //�ļ����ԣ���ʾ���̱��
 0x00,                                                               //����
 0x00,                                                               //����ʱ�����ʱ���
 0x00, 0x40,                                                         //�ļ�����ʱ��
 0x41, 0x31,                                                         //�ļ���������
 0x42, 0x31,                                                         //����������
 0x00, 0x00,                                                         //��ʼ�غŸ�λ�ֽڣ�FAT12/16����Ϊ0
 0x00, 0x40,                                                         //���дʱ��
 0x42, 0x31,                                                         //���д����
 0x02, 0x00,                                                         //��ʼ�ص�λ�ֽ� 
 0xe9, 0x00, 0x00, 0x00,                                             //�ļ�����
};

code unsigned char FILE_DATA[233]=
{"����һ�����Գ������������ļ�ϵͳ�Ƿ���������������ܿ�����Щ���֣���ô˵����ĵ����Ѿ��������޳ܵ�108�����ˣ������ϰ������ļ�ɾ��������ʽ��Ӳ�̺󣬽�Ӳ��ѹ�飬������Ҳ����Ⱦ���������30���ڻ�δ��ʼɾ���ļ�����ĵ��Խ������ñ��ϣ�"};
 
unsigned int SWAP(unsigned int temp)                                //�ߵ��ֽڽ���
{
 unsigned int i;
 i=temp;
 i<<=8;
 temp>>=8;
 i|=temp;
 return i;
}

/*unsigned long int SWAP_long(unsigned long int temp)                  //���ֽڽ���
{
 unsigned long int i;
 i=temp<<24;
  i|=(temp<<8)&0x00FF0000; 
  i|=(temp>>8)&0x0000FF00;
 i|=(temp>>24)&0x000000FF;
 return i;
}*/
 

unsigned char LSB(unsigned int temp)                                //ȡһ�������ĵ��ֽ�
{
 return (unsigned char)temp;
}

unsigned char MSB(unsigned int temp)                                //ȡһ�������ĸ��ֽ�
{
 return (unsigned char)(temp>>8);
}

code unsigned char LANGUAGE_ID[4]={0x04,0x03,0x09,0x04};            //�ַ������������õ���������        

code unsigned char device_serial_number[18]=                         //�豸���к�
 {0x12,0x03,0x32,0x00,0x30,0x00,0x37,0x00,0x31,0x00,0x30,0x00,0x39,0x00,0x38,0x00,0x32,0x00};

code DEVICE_DESCRIPTOR_STRUCT device_descriptor=     //�豸������
{
 sizeof(DEVICE_DESCRIPTOR_STRUCT),                   //�豸���������ֽ�����С
 DEVICE_DESCRIPTOR,	                             //�豸���������ͱ��
 0x1001,                                             //USB�汾��
 0x00,                                               //USB������豸�����
 0x00,                                               //USB������������
 0x00,                                               //USB������豸Э�����
 0x10,                                               //�˵�0��������С
 0x7104,                                             //���̱��
 0xf0ff,                                             //��Ʒ���
 0x0100,                                             //�豸�������
 0x00,                                               //�豸�����ַ���������
 0x00,                                               //������Ʒ�ַ���������
 0x02,                                               //�����豸���к��ַ���������
 0x01                                                //���ܵ���������
};

typedef struct _CON_INT_ENDP_DESCRIPTOR_STRUCT
{
 CONFIGURATION_DESCRIPTOR_STRUCT configuration_descriptor;
 INTERFACE_DESCRIPTOR_STRUCT  interface_descritor;
 ENDPOINT_DESCRIPTOR_STRUCT  endpoint_descriptor[ENDPOINT_NUMBER];
}CON_INT_ENDP_DESCRIPTOR_STRUCT;

code CON_INT_ENDP_DESCRIPTOR_STRUCT con_int_endp_descriptor=
{
//configuration_descriptor                                          //����������
{
 sizeof(CONFIGURATION_DESCRIPTOR_STRUCT),                           //�������������ֽ�����С
 CONFIGURATION_DESCRIPTOR,                                          //�������������ͱ��
 (sizeof(CONFIGURATION_DESCRIPTOR_STRUCT)+sizeof(INTERFACE_DESCRIPTOR_STRUCT)+sizeof(ENDPOINT_DESCRIPTOR_STRUCT)*ENDPOINT_NUMBER)*256+
 (sizeof(CONFIGURATION_DESCRIPTOR_STRUCT)+sizeof(INTERFACE_DESCRIPTOR_STRUCT)+sizeof(ENDPOINT_DESCRIPTOR_STRUCT)*ENDPOINT_NUMBER)/256,
 0x01,                                                            //ֻ����һ���ӿ�
 0x01,                                                            //�����õı��
 0x00,                                                            //iConfiguration�ֶ�
 0x80,                                                            //�������߹��磬��֧��Զ�̻���
 0xC8                                                            //�����߻�ȡ������400mA
},
//interface_descritor                                             //�ӿ�������
{
 sizeof(INTERFACE_DESCRIPTOR_STRUCT),                             //�ӿ����������ֽ�����С
 INTERFACE_DESCRIPTOR,                                            //�ӿ����������ͱ��
 0x00,                                                            //�ӿڱ��Ϊ0
 0x00,                                                            //�ýӿ��������ı��
 ENDPOINT_NUMBER,                                                 //��0�˵�����Ϊ2��ֻʹ�ö˵����˵���������
 0x08,                                                            //USB�������洢�豸
 0x06,                                                            //ʹ�õ����࣬Ϊ�򻯿�����
 0x50,                                                            //ʹ�õ�Э�飬����ʹ�õ���������Э��
 0x00                                                             //�ӿ��������ַ�������
},

//endpoint_descriptor[]
{
 {                                                                  //���˵���������
  sizeof(ENDPOINT_DESCRIPTOR_STRUCT),                             //�˵����������ֽ�����С
  ENDPOINT_DESCRIPTOR,                                            //�˵����������ͱ��
  MAIN_POINT_IN,                                                  //�˵�ţ�������˵�
  ENDPOINT_TYPE_BULK,                                             //ʹ�õĴ������ͣ���������
  0x4000,                                                         //�ö˵�֧�ֵ������ߴ磬64�ֽ�
  0x00                                                            //�ж�ɨ��ʱ�䣬������������Ч
 },
  
 {                                                                  //���˵��������
  sizeof(ENDPOINT_DESCRIPTOR_STRUCT),                             //�˵����������ֽ�����С
  ENDPOINT_DESCRIPTOR,                                            //�˵����������ͱ��
  MAIN_POINT_OUT,                                                 //�˵�ţ�������˵�
  ENDPOINT_TYPE_BULK,                                             //ʹ�õĴ������ͣ���������
  0x4000,                                                         //�ö˵�֧�ֵ������ߴ磬64�ֽ�
  0x00                                                            //�ж�ɨ��ʱ�䣬������������Ч
 }
}
};

union                                          //�����־λ
{
unsigned int Register;
struct 
{
 unsigned char bus_reset        :1;
 unsigned char suspend          :1;
 unsigned char remote_wakeup    :1;
 unsigned char int_isr          :1;
 unsigned char not_end          :1;
 unsigned char usb_idle         :1;
 unsigned char usb_busy         :1;
 unsigned char setup_packet_in  :1;
 unsigned char setup_packet_out :1;
 unsigned char set_addr         :1;
 unsigned char usb_endp0_in     :1;
 unsigned char usb_endp2_in     :1;
 unsigned char usb_endp2_out    :1;
}flags;
}usb_flags;

union                                         //�жϼĴ���
{
 unsigned char Register[2];
 struct 
  {
   unsigned char control_out_port  :1;
   unsigned char control_in_port   :1;
   unsigned char port_out_1        :1;
   unsigned char port_in_1         :1;
   unsigned char main_out_port     :1;
   unsigned char main_in_port      :1;
   unsigned char bus_reset         :1;
   unsigned char suspend_change    :1;
   unsigned char DMA_EOT           :1;
   unsigned char not_use           :7;
  }Interrupt;
}Interrupt_Register;


union                                    //�˵������״̬
{
 unsigned char Register;
 struct
 {
  unsigned char successful          :1;
  unsigned char error_code          :4;
  unsigned char setup_packet        :1;
  unsigned char data_1              :1;
  unsigned char prestatus_not_read  :1;
 }Status;
}Last_Status;


//*****************   ��ʱx���뺯�� ***********
void delay(unsigned int x)                
{
 unsigned int i;
 unsigned int j;
 for(i=0;i<x;i++)
 for(j=0;j<230;j++);
}
//********************************************


void write_usb_command(unsigned char usb_command)       //дUSB����
{
 USB_A0=USB_COMMAND_ADD;
 USB_DATA=usb_command;
 USB_WR=0;
 USB_WR=1;
 USB_DATA=0xFF;
}

void write_a_usb_data(unsigned char usb_data)        //дһ�ֽ�USB����
{
 USB_A0=USB_DATA_ADD;
 USB_DATA=usb_data;
 USB_WR=0;
 USB_WR=1;
 USB_DATA=0XFF;
}

unsigned char read_a_usb_data(void)                  //��һ�ֽ�USB����
{
 unsigned char temp;
 USB_A0=USB_DATA_ADD;
 USB_RD=0;
 temp=USB_DATA;
 USB_RD=1;
 return temp;
}


void read_interrupt_register(void)             //��USB�жϼĴ���
{
 write_usb_command(Read_Interrupt_Register);
 Interrupt_Register.Register[0]=read_a_usb_data();
 Interrupt_Register.Register[1]=read_a_usb_data();
}

void set_usb_addr(unsigned char addr)          //����USB��ַ
{
 write_usb_command(Set_Address);
 write_a_usb_data(0x80|addr);
}

void set_endpoint_enable(void)                  //�˵�ʹ��
{
 write_usb_command(Set_Endpoint_Enable);
 write_a_usb_data(0x01);
}

unsigned char select_endpoint(unsigned char endp)   //ѡ���ն˵�
{
 write_usb_command(Select_EndPoint+endp);
 return read_a_usb_data();
}

unsigned char  read_last_status(unsigned char endp)   //���˵����״̬
{
 write_usb_command(Read_Last_Status+endp);
 return read_a_usb_data();
}

void set_endpoint_status(unsigned char endp,unsigned char status)  //���ö˵�״̬
{
 write_usb_command(0x40+endp);
 write_a_usb_data(!status);
}

unsigned char read_endpoint_status(unsigned char endp)           //���˵�״̬
{
 write_usb_command(0x80+endp);
 return read_a_usb_data();
}

void clear_buffer(void)                        //�建�壬�ڶ�ȡ�������ݺ����
{
 write_usb_command(Clear_Buffer);
}

void validate_buffer(void)                    //������������Ч����д��������
{
 write_usb_command(Validate_Buffer);
}

void ack_setup(unsigned char endp)            //Ӧ������
{
 select_endpoint(endp);
 write_usb_command(Ack_Setup);
}

void error(unsigned char number)             //������
{
// unsigned char Error[]={"ERROR: "};
// prints(Error);
// printc(number);
 number=0;
}

unsigned char read_endpoint_buff(unsigned char endp,unsigned char len,unsigned char * buff)   //���ն˵㻺��
{
 unsigned char i,j;
 read_last_status(endp);
 if(!(select_endpoint(endp))&0x01){error(0); return 0;}
 read_endpoint_status(endp);
 write_usb_command(Read_Buffer);
 read_a_usb_data();
 j=read_a_usb_data();
 if(j>len)
  j=len;
 for(i=0;i<j;i++)
  {
   USB_RD=0;
   *(buff+i)=USB_DATA;
   USB_RD=1;
  }
 clear_buffer();
 return j;
}

unsigned char write_endpoint_buff(unsigned char endp,unsigned char len,unsigned char * buff)    //д�ն˵㻺��
{
 unsigned char i;
 read_last_status(endp);
 select_endpoint(endp);
 write_usb_command(Write_Buffer);
 write_a_usb_data(0);
 write_a_usb_data(len);
 for(i=0;i<len;i++)
  {
   USB_DATA=*(buff+i);
   USB_WR=0;
   USB_WR=1;
  }
 USB_DATA=0XFF;
 validate_buffer();
 return len;
}

void disconnect_usb(void)
{
 write_usb_command(0xf3);
 write_a_usb_data(0x0e);
 write_a_usb_data(0x47);
 delay(100);
}

void connect_usb(void)                            //����USB
{
 write_usb_command(0xf3);                         //��ʼ��USBD12
 write_a_usb_data(0x1e);                          //����USB
 write_a_usb_data(0x47);                          //����Ƶ��
}

void init_usb(void)                              //��ʼ��USB
{
 set_usb_addr(0);
 set_endpoint_enable();
}

void usb_bus_reset(void)                         //USB���߸�λ����
{
// code unsigned char Usb_Bus_Reset[]={"USB BUS RESET"};
// prints(Usb_Bus_Reset);
 usb_flags.Register=0;
 set_endpoint_enable();
 SCSI.Status.Command=1;
 SCSI.Status.Data=0;
 csw.dCSWSignature=0x55534253;

}

void usb_bus_suspend(void)                       //USB���߹�����
{
// code unsigned char Usb_Bus_Suspend[]={"USB BUS SUSPEND"};
// prints(Usb_Bus_Suspend);
// send_to_comport(10);
}

void set_usb_address(void)                       //���õ�ַ
{
 usb_flags.flags.set_addr=1;
 while(select_endpoint(1)&0x01);
 write_endpoint_buff(1,0,0);
 set_usb_addr(Control_Data.DeviceRequest.wValue);
 usb_flags.flags.usb_endp0_in=0;
 usb_flags.flags.setup_packet_out=0;
}

void get_status(unsigned char receiver)           //��ȡ״̬��Ӧ
{
 unsigned char status[2];
 switch (receiver)
  {
   case 0:                        //��ȡ�豸״̬
    status[0]=0x00;
    status[1]=0x00;
    break;
   case 1:                        //��ȡ�ӿ�״̬
    status[0]=0x00;
    status[0]=0x00;
    break;
   case 2:                        //��ȡ�˵�״̬
    status[0]=0x00;
    status[1]=0x00;
    break;
   }
  write_endpoint_buff(1,2,status);
  usb_flags.flags.usb_endp0_in=0;
}

void clear_feature(unsigned char receiver)    
{
 receiver=0;
 write_endpoint_buff(1,0,0);
 usb_flags.flags.usb_endp0_in=0;
 usb_flags.flags.setup_packet_out=0;
}

void set_feature(unsigned char receiver)
{
 receiver=0;
 write_endpoint_buff(1,0,0);
 usb_flags.flags.usb_endp0_in=0;
 usb_flags.flags.setup_packet_out=0;
}

void set_descriptor(void)
{
 usb_flags.flags.usb_endp0_in=0;
 usb_flags.flags.setup_packet_out=0;
}

void set_configuration(void)
{
 write_endpoint_buff(1,0,0);
 usb_flags.flags.usb_endp0_in=0;
 usb_flags.flags.setup_packet_out=0;
}

void get_configuration(void)               //��ȡ����״̬
{
 unsigned char value=0x01;
 write_endpoint_buff(1,1,&value);
 usb_flags.flags.usb_endp0_in=0;
}

void set_interface(void)                   //���ýӿ�
{
 write_endpoint_buff(1,0,0);
 usb_flags.flags.usb_endp0_in=0;
 usb_flags.flags.setup_packet_out=0;
}

void get_interface(void)                  //��ȡ�ӿ�״̬
{
 unsigned char value=0x01;
 write_endpoint_buff(1,1,&value);
 usb_flags.flags.usb_endp0_in=0;
}

void get_max_LUN(void)                   //��ȡ��������߼���Ԫ��
{
 unsigned char max_LUN=MAX_LUN;          //ֻ��һ���߼���Ԫ
 write_endpoint_buff(1,1,&(max_LUN));
 usb_flags.flags.usb_endp0_in=0; 
}

void mass_storage_reset(void)            //USB�������洢�豸��λ
{
 write_endpoint_buff(1,0,0);
 usb_flags.flags.usb_endp0_in=0;
 usb_flags.flags.setup_packet_out=0;
 SCSI.Status.Command=1;
 SCSI.Status.Data=0;
}


void get_descriptor(void)                    //��ȡ������
{
    if(!usb_flags.flags.not_end)
     {
      switch(MSB(Control_Data.DeviceRequest.wValue))
	   {
        case DEVICE_DESCRIPTOR:
	         Control_Data.wCount=sizeof(DEVICE_DESCRIPTOR_STRUCT);
 	         Control_Data.pData=(unsigned char *)(&device_descriptor);
             break;
        case CONFIGURATION_DESCRIPTOR:
             Control_Data.wCount=SWAP(con_int_endp_descriptor.configuration_descriptor.wTotalLength);
			 Control_Data.pData=(unsigned char *)(&con_int_endp_descriptor);
             if(Control_Data.wLength<Control_Data.wCount)Control_Data.wCount=Control_Data.wLength;
             break;
		case STRING_DESCRIPTOR:
		     if(LSB(Control_Data.DeviceRequest.wValue)==0)
			  {
			   Control_Data.wCount=LANGUAGE_ID[0];
               Control_Data.pData=LANGUAGE_ID;
			  }
		     if(LSB(Control_Data.DeviceRequest.wValue)==2)
			  {
			   Control_Data.wCount=device_serial_number[0];
			   Control_Data.pData=device_serial_number;
			  }
			 break;
	   	}
      if(Control_Data.wLength<Control_Data.wCount)Control_Data.wCount=Control_Data.wLength;
	 }

    if(Control_Data.wCount>=MAX_CONTROL_DATA_SIZE)
     {
	  write_endpoint_buff(1,MAX_CONTROL_DATA_SIZE,Control_Data.pData);
	  Control_Data.pData+=MAX_CONTROL_DATA_SIZE;
      Control_Data.wCount-=MAX_CONTROL_DATA_SIZE;
	  if(usb_flags.flags.set_addr)usb_flags.flags.not_end=1;
	   else usb_flags.flags.usb_endp0_in=0;
	  return;
	 }
    else
     {
	  write_endpoint_buff(1,Control_Data.wCount,Control_Data.pData);
      usb_flags.flags.setup_packet_in=0;
	  usb_flags.flags.usb_endp0_in=0;
	  return;
	 }      
}

void endp0_out(void)                            //�ն˵�0����жϴ���
{
 Last_Status.Register=read_last_status(0);
 if(Last_Status.Status.setup_packet)
  {
   Control_Data.wLength=0;
   Control_Data.wCount=0;
   if(read_endpoint_buff(0,sizeof(Control_Data.DeviceRequest),(unsigned char *)(&(Control_Data.DeviceRequest)))!=sizeof(REQUESTCMD))
    {
	 set_endpoint_status(0,0);
	 set_endpoint_status(1,0);
	 return;
	}
  Control_Data.DeviceRequest.wValue=SWAP(Control_Data.DeviceRequest.wValue);
  Control_Data.DeviceRequest.wIndex=SWAP(Control_Data.DeviceRequest.wIndex);
  Control_Data.DeviceRequest.wLength=SWAP(Control_Data.DeviceRequest.wLength);

  ack_setup(0);
  ack_setup(1);
  Control_Data.wLength=Control_Data.DeviceRequest.wLength;
  usb_flags.flags.not_end=0;
  usb_flags.flags.usb_endp0_in=1;
  usb_flags.flags.setup_packet_in=0;
  usb_flags.flags.setup_packet_out=0;
  if(Control_Data.DeviceRequest.bmRequestType&0x80){usb_flags.flags.setup_packet_in=1;return;}
  else {usb_flags.flags.setup_packet_out=1;return;}
 }
 else
  {
   select_endpoint(0);
   clear_buffer();
  }
}

void endp0_in(void)                                        //�ն˵�0���봦��
{
 read_last_status(1);
 if(usb_flags.flags.setup_packet_in||usb_flags.flags.not_end)
  {
   if((Control_Data.DeviceRequest.bmRequestType==0xA1)&&(Control_Data.DeviceRequest.bRequest==0xFE))
    get_max_LUN();
   switch(Control_Data.DeviceRequest.bmRequestType&0x7B)
    {
	 case 0:
	   switch (Control_Data.DeviceRequest.bRequest)
	    {
	     case 0: get_status(0);break;
	     case 6: get_descriptor();break;
	     case 8: get_configuration();break;
		 default:break;
	    }
	 case 1:
	   switch (Control_Data.DeviceRequest.bRequest)
	    {
         case  0: get_status(1);break;
	     case 10: get_interface();break;
         default: break;
		}
	 case 2:
	  {
       if(Control_Data.DeviceRequest.bRequest==0)get_status(2);  
	  }
	 default: break;
    }
  return;
  }
 if(usb_flags.flags.setup_packet_out) 
  {
   if((Control_Data.DeviceRequest.bmRequestType==0x21)&&(Control_Data.DeviceRequest.bRequest==0xFF))
    mass_storage_reset();

   switch(Control_Data.DeviceRequest.bmRequestType&0x03)
    {
	 case 0:
     switch (Control_Data.DeviceRequest.bRequest)
      {
	   case     CLEAR_FEATURE: clear_feature(0);break;
	   case       SET_FEATURE: set_feature(0);break;
	   case       SET_ADDRESS: set_usb_address();break;                  //set_address	           
	   case    SET_DESCRIPTOR: set_descriptor();break;                //set_descriptor
	   case SET_CONFIGURATION: set_configuration(); break;         //set_configurat               	          
  	   default: break;
	  }break;
	 case 1:
      switch (Control_Data.DeviceRequest.bRequest)
	   {
	    case CLEAR_FEATURE:  clear_feature(1);break;
	    case   SET_FEATURE:  set_feature(1); break;
        case SET_INTERFACE: set_interface();break;
	    default: break;
        }break;
	 case 2:
	  switch (Control_Data.DeviceRequest.bRequest)
	   {
	    case CLEAR_FEATURE: clear_feature(2);break;
	    case   SET_FEATURE: set_feature(2);break;
        default: break;      
	   }break;
	 default: break;
    }
  }
}

/*void endp1_out(void)
{
 printc(0xcc);
}*/


/*void endp1_in(void)
{
 printc(0xcc);
}*/

void Return_CSW(unsigned long int DataResidue,unsigned char status)          //����CSW���ݰ�
{
  
  csw.dCSWTag=cbw.dCBWTag;
  csw.dCSWDataResidue=DataResidue;
  csw.bCSWStatus=status;
  while(select_endpoint(5)&0x01);
  write_endpoint_buff(5,sizeof(CSW),(unsigned char *)(&csw));
  usb_flags.flags.usb_endp2_in=0;
  usb_flags.flags.usb_endp2_out=0;
  SCSI.Status.Command=1;
  SCSI.Status.Data=0;
}

void write_10(void)                                                         //SCSIд����
{
 unsigned long int LBA,Byte_Count;
 unsigned char i;
  if(SCSI.Status.Data)
  {
   for(i=0;i<4;i++)
   {
    LBA<<=8;
    LBA+=cbw.CBWCB[2+i];
   }
//   pData=get_physical_addr(LBA);
   Byte_Count=0;
   Byte_Count+=cbw.CBWCB[7];
   Byte_Count<<=8;
   Byte_Count+=cbw.CBWCB[8];
   Byte_Count<<=9;
 while(usb_flags.flags.usb_endp2_out)
  {
   while(!(select_endpoint(4)&0x01));
	if(Byte_Count>64)
	 {
	  read_endpoint_buff(4,64,buffer);
	  Byte_Count-=64;
	 }
	else 
	 {
      read_endpoint_buff(4,Byte_Count,buffer);
      Return_CSW(0x00,SUCCESS);
	 }
   }
 }
}

void endp2_out(void)                                                     //���˵��������
{
// printc(0xcc);
 if(SCSI.Status.Command)
  {
   if(read_endpoint_buff(4,sizeof(CBW),(unsigned char *)(&cbw))!=sizeof(CBW)){error(2);return;}
   if(cbw.dCBWSignature!=0x55534243){error(3);return;}
   SCSI.Status.Command=0;
   SCSI.Status.Data=1;
   if(cbw.bmCBWFlags&0x80)
    {
	 usb_flags.flags.usb_endp2_in=1;
	 usb_flags.flags.usb_endp2_out=0;
	}
   else
    {
	 usb_flags.flags.usb_endp2_in=0;
	 usb_flags.flags.usb_endp2_out=1;
     switch(cbw.CBWCB[0])
      {
       case        Write_10: write_10();break;
       case Test_Unit_Ready: Return_CSW(0x00,SUCCESS);break; 
       case          Verify: Return_CSW(0x00,SUCCESS);break;
	   default             : Return_CSW(cbw.dCBWDataTransgerLength,FAIL);break;
	  }
	}
  }
 else
  {
   read_last_status(4);
   select_endpoint(4);
   clear_buffer();
  }
}

unsigned char *get_physical_addr(unsigned long int LBA)                        //��LBA��ȡ�����ַ
{
 if(LBA==0)return DBR;
 if(LBA==2)return FAT;
 if(LBA==0x40)return ZERO;
 if(LBA==0X0A)return FAT;
}


void read_10(void)                                                              //SCSI������
{
 unsigned long int LBA=0,Byte_Count;
 unsigned long int count=0;
 unsigned char i;
 if(SCSI.Status.Data)
  {
   for(i=0;i<4;i++)
   {
    LBA<<=8;
    LBA+=cbw.CBWCB[2+i];
   }
   pData=get_physical_addr(LBA);
   Byte_Count=0;
   Byte_Count+=cbw.CBWCB[7];
   Byte_Count<<=8;
   Byte_Count+=cbw.CBWCB[8];
   Byte_Count<<=9;
   i=0;
  while(usb_flags.flags.usb_endp2_in)
   {
    if(LBA==0)
       if(count<512)pData=DBR+count;
	   else 
	    if(count==512)pData=FAT;
		else pData=ZERO;
	if(LBA==2)
//	   if(count<64)pData=FAT;
//	   else
 //	    if(count==512*7)pData=FAT;
 //      else
		 pData=ZERO;
 	if(LBA==8)
 //	    if(count<512)pData=ZERO;
 //		else 
 //		 if(count==512)pData=FAT;
 //		 else 
		  pData=ZERO;
     if(LBA==0x10)
	    if(count==512)pData=FAT;
	   	 else pData=ZERO;
	if(LBA==0x21)
	 if(count==0)
//	  else
       pData=ROOT_DIR;
	   else pData=ZERO;

	if(LBA==0x40)
	  if(count<240)pData=FILE_DATA+count;
	  else 
	    pData=ZERO;

   while(select_endpoint(5)&0x01);
   if(Byte_Count>MAX_BULK_DATA_SIZE)
   {
    write_endpoint_buff(5,MAX_BULK_DATA_SIZE,pData);
//    pData+=MAX_BULK_DATA_SIZE;
    Byte_Count-=MAX_BULK_DATA_SIZE;
//    SCSI.Status.Data=1;
   }
  else
   {
    write_endpoint_buff(5,Byte_Count,pData);
    Return_CSW(0x00,SUCCESS);
   }
  count+=64;
  }
 }
 else error(2);return;
}

void endp2_in(void)                                      //���˵����봦��
{
    switch(cbw.CBWCB[0])
    {
     case              Read_10: read_10();break;
     case              Inquiry: write_endpoint_buff(5,0x24,DISK_INF);Return_CSW(0x00,SUCCESS); break;
     case        Read_Capacity: write_endpoint_buff(5,0x08,DISK_CAPACITY);Return_CSW(0x00,SUCCESS);break;
     case Read_Format_capacity: write_endpoint_buff(5,0x00,0x00);Return_CSW(cbw.dCBWDataTransgerLength,FAIL);break;
     case        Request_Sense: write_endpoint_buff(5,0x12,SENSE);Return_CSW(0x00,SUCCESS);break;
	 case       Medium_Removal: Return_CSW(0x00,SUCCESS);break;
	 case                 0x1a: write_endpoint_buff(5,0x00,0x00);Return_CSW(cbw.dCBWDataTransgerLength,FAIL);break;
     default                  : write_endpoint_buff(5,0x00,0x00);Return_CSW(cbw.dCBWDataTransgerLength,FAIL);break;
	}
}

void main(void)
{
 disconnect_usb();
// delay(1000);
// init_comport(57600);
 init_usb();
 connect_usb();
// send_to_comport('O');
// send_to_comport('K');
  
 while(1)
  { 
   if(!USB_INT)
    {
  	 read_interrupt_register();
//      delay(10);
//      printc(Interrupt_Register.Register[0]);
	 if(Interrupt_Register.Interrupt.bus_reset){usb_bus_reset();continue;}
	 if(Interrupt_Register.Interrupt.suspend_change){usb_bus_suspend();continue;}
         if(Interrupt_Register.Interrupt.control_out_port){endp0_out();continue;}
	 if(Interrupt_Register.Interrupt.control_in_port&&usb_flags.flags.usb_endp0_in){endp0_in();continue;}
//	 if(Interrupt_Register.Interrupt.port_out_1)endp1_out();
//	 if(Interrupt_Register.Interrupt.port_in_1)endp1_in();
	 if(Interrupt_Register.Interrupt.main_out_port){endp2_out();continue;}
	 if(Interrupt_Register.Interrupt.main_in_port&&usb_flags.flags.usb_endp2_in){endp2_in();continue;}
    }
  }
}
