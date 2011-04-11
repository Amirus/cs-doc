/************************************************************************************************
*                           
*                                 USB118 C51���Գ���
*                                                               Version 1.1
*                ������̩�����������ι�˾��Xi'an DATA Electronic Co,Ltd��      
*   ��ַ�� http://www.dataie.com 
*   �绰�� 029-85272421 , 88022900
*  
*                                             ������̩�����������ι�˾        2008.05
*************************************************************************************************/


/************************************************************************************************
*
*  U���ļ���дģ�� ���ֽ�Ϊ��λ����U���ļ���д��
*  MCS-51��Ƭ��C����ʾ������ 
*  Ӳ�����ӷ�ʽ: 4���ƴ��ڣ���Դ���أ����룬����� 
*
*************************************************************************************************/

#include <AT89X52.h>

#define uchar  unsigned  char
#define uint   unsigned  int 
	
uchar  commd[66]={0x01,0x02,0x43,0x44,0x45,0x46}; //send and receive command buffer
uchar  idata dat1[32]={0x61,0x62,0x63,0x64,0x65,0x66}; //send  and receive databuffer
long int sm; 
uchar Command_status=0;	  //receive status 

/*---------------------------------
�������ܣ�send data 
��ڲ�����
---------------------------------*/
void int_data1(uchar da)
{
 uchar dat;
 for(dat=0; dat<32; dat+=2)
   {
    dat1[dat]=da+dat;
   }
}
 /*
void int_data2(uchar da)
{
 uchar dat;
 for(dat=1; dat<32; dat+=2)
  {
   dat1[dat]=da;
  }
}
/*-------------------------------------------
            �����ʼ������
�������ܣ���ʼ������
��ڲ�����ia��Ҫ��commd[]�����ʼ��ֵ
���ڲ�����
-------------------------------------------*/
void intia(uchar ia)
{
  uchar t;
  for(t=0; t<66; t++)
   {
    commd[t]=ia;
   }
}


/*--------------------------------------------------
               ��ʱ����  ��ʱsec*01ms
�������ܣ���ʱ function ,when frenquency 24M , 0.1ms
��ڲ�����c ����ʱ���ٸ�0.1ms
-----------------------------------------------------*
void delay(uint sec) 
{
 uint i;
 uchar j;
 for(i=0; i<sec; i++)
   for(j=0; j<100; j++)
  	;	 	   
}

 /*------------------------------------------------------
               ��ʱ����  ��ʱ=sec*15.1ms
�������ܣ���ʱ function ,when frenquency 24M , 15.1ms
��ڲ�����c ����ʱ���ٸ�15.1ms
--------------------------------------------------------*/
void Del(int sec) 
{
 uint i;
 uchar j, b; 
 for(i=0; i<sec; i++)
   for(j=0; j<100; j++)
     for(b=0; b<99; b++)
	   ;	   
}

/*----------------------------------------
�������ܣ�initialize serial function

-------------------------------------------
void serial_int()
{
  EA=0;		 //���жϽ�ֹ
  ES=0;		 //serial interrupt forbid
  TMOD=0x20; //��ʱ��T1ʹ�ù�����ʽ2
  TH1=0xF3;	//���ó�ֵ
  TL1=0xF3;
  TR1=1;
  PCON=0x80;	//SMOD=1
  SCON=0x50;	//������ʽ1��9600bit/s 
  TI=1;
  REN=1;
}
*/

/*---------------------------------------------------
                ����ͨѶ�ڳ�ʼ������ 
�������ܣ���ʼ��uart   
��ڲ����������ʣ�9600�� 19200�� 57600��
----------------------------------------------------*/ 
void Init_COMM(uint sp)
{
	     SCON = 0x53;
	     PCON = 0x80 | PCON;
         T2CON=0x30;
	     switch(sp)
	     {
	       case 9600:{// 9600 @ 24MHz: 24000000/(32*(65536-(RCAP2H,RCAP2L)))
                        RCAP2H=0xFF;		
	                    RCAP2L=0xB2;
					  }break;
		   case 19200:{// 19200 @ 24MHz: 24000000/(32*(65536-(RCAP2H,RCAP2L)))
			            RCAP2H=0xFF;		
	                    RCAP2L=0xD9;
					  }break;
           case 57600:{// 57600 @ 24MHz: 24000000/(32*(65536-(RCAP2H,RCAP2L)))
			            RCAP2H=0xFF;		
	                    RCAP2L=0xF3;
					  }break;
          }
          TI=1;
	      RI=0;
	      TR2=1;
		  
}
/*-----------------------------------------------------
                URAT�����ʽ0xaa+0xbb+command+data
�������ܣ��Ƿ���ͬ����0xaa+0xbb
����˵����syn is send string 
-------------------------------------------------------*/
void serial_syn(  )
{
  uchar i;
  uchar syn;
  i=0;
  syn=0xaa;
  TI=0;
  while(i<2)
  { 
    SBUF=syn;
    while(!TI);
	TI=0;		
	syn=0xbb;
	i++;   
  }
}

/*--------------------------------------------------------
                   ���ڽ���һ���ֽ�����
�������ܣ�����һ���ֽ�����
---------------------------------------------------------*/
void Send_Data(uchar send)
{
   
   SBUF=send;
   while(!TI);   
   TI=0;	              			            
}
/*---------------------------------------------------------------------------------------------------
                     ���ڷ��ͺ���command(64)+data(0-2048)
��������: serial send data
��ڲ���: *s_command ������ָ�룬��Э��Ҫ����64���ֽ�Ϊһ֡��*s_dat������ָ�룬
          s_num��Ҫ���͵������ֽ�����0-2048��
 ----------------------------------------------------------------------------------------------------*/
void serial_send(uchar *s_command,  uchar *s_dat, uint s_num)
{
  uint s;
  for(s=0; s<64; s++)   	 
   {
     //SBUF=*s_command;
	// while(!TI);
   	// TI=0;
	 Send_Data(*s_command);
	 s_command++;  
    }
   for(s=0; s<s_num; s++)
	{
 	 //SBUF=*s_dat;
	 //while(!TI);
   	 //TI=0;
	 Send_Data(*s_dat);
	 s_dat++; 	
   	}
   TI=0;
   RI=0;

   EA=1;
   ES=1;    
}

/*--------------------------------------------------------
                   ���ڽ���һ���ֽ�����
�������ܣ�����һ���ֽ�����
---------------------------------------------------------*/
unsigned char Receive_Data()
{
         uchar receive;
		 while(!RI);
		 receive=SBUF;
		 RI=0;	              			  
         return (receive);           
}
/*-------------------------------------------------------------------------------------------------
                    ���ڽ���һ֡���ݣ����ݸ�ʽ command(64)+data(0-16384)
�������ܣ�����һ֡����
��ڲ�����*r_command�ǽ�����Ӧ����֡����ָ�룬*r_dat�ǽ�������ָ�룬r_num��������ָ�������ݸ�����
���ڲ�����return 0 ��ʾ��������, 1 ��ʾ����һ֡��� 
---------------------------------------------------------------------------------------------------*/
unsigned char Receive_One(uchar *r_command,  uchar *r_dat)
{
         
  uchar i;
  uint r_num;
  r_num=0;
  for(i=0; i<3; i++)
   {
    *r_command=Receive_Data();
	 r_command++;
   }
  if(commd[0]==0xaa)	 //������յ�0xaa+0xbb+0x01 ������Ϣ��ʽ�ǣ� 0xaa + 0xbb +0x01
	  return 0;
  for(i=3;i<63;i++)      //���Ǵ�����Ϣʱ������������һ֡��64byte��    
   {
    *r_command=Receive_Data();
   	r_command++;
    }
 		 																		 
  if((commd[0]==0xB0))	 //�����read �����������ݷ���
    {
    	r_num=commd[6];	 //data for r_num BYTE    
		r_num<<=8;
		r_num|=commd[5];
     }
   if(commd[0]==0x20) 	 //����� list �����������ݷ���
	 {
       r_num=commd[5];	 //data for r_num BYTE    
       r_num<<=8;
	   r_num|=commd[4];
	  }    		   
   for(i=0;i<r_num;i++)    //receive data,   r_num(0-16384)
     {
      *(r_dat+i)=Receive_Data();
     }		 
    return 1;
}
/*----------------------------------------------
�������ܣ�serial receive  data
����˵����ͬserial_send()
------------------------------------------------*
unsigned char serial_rec(unsigned char *r_command,  unsigned char *r_dat,  unsigned int r_num)
{
 unsigned int r, receive_data;
 //serial_int();
 
 for(r=0; r<3; r++)
   {
    while(!RI);
	receive_data=SBUF;
	RI=0;
    *r_command=receive_data;		 		 		 
	r_command++;	
   	  
   }
   delay(10);
  if(commd[0]==0xaa)
      return 0;

 for(r=3; r<64; r++)	 
   {
	while(!RI);
	receive_data=SBUF;
	RI=0;
	*r_command=receive_data;		 		 		 
	r_command++;
	
   	  
   }

 for(r=0; r<r_num; r++)
   {
	while(!RI);
	receive_data=SBUF;
	RI=0; 
	*r_dat=receive_data;
    r_dat++;
	
   	 
   }
    return 1;	 
  
}
 
/*-------------------------------------------------------------------------------------------
            ����豸�Ƿ�����
��������: USB118 detect u
���ڲ���: USB118T_Detect commd[0]= 0x04, commd[1] �Ǽ���� 0x01�д��̣�0x00û�д��̡� 
------------------------------------------------------------------------------------------- */
void USB118_Detect()
{
  intia(0x20);		//intialize	command data group

  commd[0]=0x01;	 //USB118R_Detect command 0x01
  commd[1]=0x00;  
    
  serial_syn();
  serial_send(commd, 0, 0);
}

/*-----------------------------------------------------------------------------------------------------------------------
		  �����ļ�����Ŀ¼
�������ܣ�create file or makedir
��ڲ�����tpye is 0x07(file) or 0x32(makedir),na1--an8 feil name, tp1--tp3 feil tpye
���ڲ�����USB118_Create  ,Create ����command[0]=0x0A, command [1]  01H��ʾ�ɹ���00H��ʾʧ�ܣ�command[2]-[3]��ʾ������
      and USB118_MakeDir ,MakeDir����command[0]=0x42��command [1]  01H��ʾ�ɹ���00H��ʾʧ�ܣ�command[2]-[3]��ʾ������ 
----------------------------------------------------------------------------------------------------------------------- */
void USB118_Crefiledir( uchar tpye,  uchar na1, uchar na2, uchar na3, uchar na4, 
                         uchar na5,  uchar na6, uchar na7, uchar na8, uchar tp1,
					   	 uchar tp2,  uchar tp3 )
{
 intia(0);
 commd[0]=tpye;		//create file(07H) or dir(32H)
 commd[1]=na1;
 commd[2]=na2;
 commd[3]=na3;
 commd[4]=na4;
 commd[5]=na5;
 commd[6]=na6;
 commd[7]=na7;
 commd[8]=na8;

 commd[9]=tp1;
 commd[10]=tp2;
 commd[11]=tp3;

 commd[12]=0x20;	//create file attribute
 commd[23]=0x38;    //create file time
 commd[24]=0x21;
 commd[33]=0;

 serial_syn();
 serial_send(commd, 0, 0);  
 //serial_rec(commd, 0, 0);
}

/*------------------------------------------------------------------
�������ܣ� open file
��ڲ�����The na1-8 is file name, the tp1-3 is file type.
���ڲ�����USB118_Open  ,Open ����command[0]=0x0D, command [1]  01H��ʾ�ɹ���00H��ʾʧ�ܣ�
          command[2]-[3]��ʾ������ 
------------------------------------------------------------------*/
void  USB118_Open(uchar na1, uchar na2, uchar na3, uchar na4,uchar na5, uchar na6, uchar na7, uchar na8, 
                  uchar tp1, uchar tp2, uchar tp3)
					   	   
{
 intia(0x20);
 commd[0]=0x06;	 //open file 
 commd[1]=na1;
 commd[2]=na2;
 commd[3]=na3;
 commd[4]=na4;
 commd[5]=na5;
 commd[6]=na6;
 commd[7]=na7;
 commd[8]=na8;

 commd[9]=tp1;
 commd[10]=tp2;
 commd[11]=tp3;

 serial_syn();
 serial_send(commd, 0, 0);
 //serial_rec(commd, 0, 0);	
}

/*-----------------------------------------------------------------------------
             д������
�������ܣ�when open  or create file, write data
��ڲ�����*wdata ��Ҫд���������ָ��ͷ, wnum��д�����ݵĳ���(0--2048)
-------------------------------------------------------------------------------*/		 
void USB118_Write(uchar *wdata, uint wnum)
{
  
  uchar n;
  uint data_num;
  intia(0);
  data_num=wnum;
  n=wnum&0xff;
  wnum>>=8;
  
  commd[0]=0x09;	 //write data
  commd[1]=0;
  commd[2]=n;
  commd[3]=(uchar)wnum;
  				    
  serial_syn();
  serial_send(commd, wdata, data_num);

  //serial_rec(commd, 0, 0);
}  	  
/*-----------------------------------------------
��������:������Ŀ¼
��ڲ���:na1-8��Ҫ�����Ŀ¼��
------------------------------------------------*
void USB118_InDir(uchar na1, uchar na2, uchar na3, uchar na4,uchar na5, uchar na6, uchar na7, uchar na8)
{
 intia(0x20);

 commd[0]=0x33;		   //in dir
 commd[1]=na1;
 commd[2]=na2;
 commd[3]=na3;
 commd[4]=na4;
 commd[5]=na5;
 commd[6]=na6;
 commd[7]=na7;
 commd[8]=na8;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*------------------------------       
��������:���ص�ǰĿ¼����һ��
-------------------------------*
void USB118_OutDir()
{
 intia(0x20);
 
 commd[0]=0x34;	  // out dir

 serial_syn();
 serial_send(commd, 0, 0);
}

/*------------------------------------
��������:���ظ�Ŀ¼
--------------------------------------*
void USB118_RootDir()
{
 intia(0x20);

 commd[0]=0x35;	  // return root dir

 serial_syn();
 serial_send(commd, 0, 0);
}

/*----------------------------------
��������:read data
��ڲ���:length��ȡ���ݵĳ���
-----------------------------------*
void USB118_Read(uint length)
{
 uchar leng;
 intia(0x20);

 leng=(uchar)length&0xff;
 length>>=8; 
 commd[0]=0x08;	    //read file
 commd[1]=leng;
 commd[2]=(uchar)length;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*----------------------------------
��������:�о�Ŀ¼���ļ����ļ���
-----------------------------------*
void USB118_List()
{
 intia(0x20);
 
 commd[0]=0x10;
 commd[1]=0;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*--------------------------------------------------------------
��������:ɾ��ָ���ļ�����Ŀ¼
��ڲ���:na1-8��Ҫɾ�����ļ���, tp1-3��Ҫɾ�����ļ�����չ��
--------------------------------------------------------------*
void USB118_Remove(uchar na1, uchar na2, uchar na3, uchar na4,uchar na5, uchar na6, uchar na7, uchar na8, 
                  uchar tp1, uchar tp2, uchar tp3)
{
 intia(0x20);

 commd[0]=0x11;
 commd[1]=0;
 commd[2]=na1;
 commd[3]=na2;
 commd[4]=na3;
 commd[5]=na4;
 commd[6]=na5;
 commd[7]=na6;
 commd[8]=na7;
 commd[9]=na8;

 commd[10]=tp1;
 commd[11]=tp2;
 commd[12]=tp3;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*----------------------------------------
��������:��ȡ���̿ռ�
----------------------------------------*
void USB118_GetCapacity()
{
  intia(0x20);

  commd[0]=0x12;
  commd[1]=0;

  serial_syn();
  serial_send(commd, 0, 0);
}

/*--------------------------------------------------
��������:�ƶ�ָ��λ��
��ں���:sp ��ָ���ƶ�����λ�� (0--4294967295)
---------------------------------------------------*/
void USB118_SetFilePointer(long int sp)
{
 //intia(0x20);

 commd[0]=0x15;
 commd[1]=0;
 commd[2]=(uchar)sp;
 sp>>=8;
 commd[3]=(uchar)sp;
 sp>>=8;
 commd[4]=(uchar)sp;
 sp>>=8;
 commd[5]=(uchar)sp;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*---------------------------------
��������:��ȡϵͳ�汾��
-----------------------------------*
void USB118_GetVersion()
{
 intia(0x20);

 commd[0]=0x30;
 commd[1]=0;

 serial_syn();
 serial_send(commd, 0, 0);
} 

/************************************************
������
*************************************************/
main()
{

 Del(300);
 //intia(0x20);  
 Init_COMM(9600);

 while(1)
 {
  USB118_Detect();
  Del(50);
 // if(Command_status==0x01)
	if((commd[0]==0x04)&(commd[1]==0x01)) 
	  {
	   break;
	   }
   
 }
 while(1)
 {
  USB118_Crefiledir( 0x07,  0xB4, 0XEF, 0xCC, 0xA9, 'U', 'S', 'B', 0x20, 'T', 'X', 'T');
  Del(50);
 // if(Command_status==0x01)
	if((commd[0]==0x0A)&(commd[1]==0x01)) 
	  {
	   break;
	   }
   
 } 	  
 Del(10);	
   while(1)
 {
  USB118_Open(0xB4, 0XEF, 0xCC, 0xA9, 'U', 'S', 'B', 0x20, 'T', 'X', 'T');
  Del(50);
 // if(Command_status==0x01)
	if((commd[0]==0x0D)&(commd[1]==0x01)) 
	  {
	   sm=(long int)commd[35];
	   sm<<=8;
	   sm|=(long int)commd[34];
	   sm<<=8;							  
	   sm|=(long int)commd[33];
	   sm<<=8;
	   sm|=(long int)commd[32];
	   break;
	   }
   
 }
 Del(10);
  while(1)
 {
  USB118_SetFilePointer(sm);
  Del(50);
 // if(Command_status==0x01)
	if((commd[0]==0x25)&(commd[1]==0x01)) 
	  {
	   break;
	   }
   
 } 		
 Del(10); 

 while(1)
 {
  int_data1(0x41);	 //Tab����ASCII��09H
  dat1[0]=0xCE;	  //��
  dat1[1]=0xF7;	  

  dat1[2]=0xB0;	  //��
  dat1[3]=0xB2;

  dat1[4]=0xB4;	  //��
  dat1[5]=0xEF;	 

  dat1[6]=0xCC;	  //̩
  dat1[7]=0xA9;

  dat1[8]=0xB5;	  //��
  dat1[9]=0xe7;

  dat1[10]=0xD7;  //��
  dat1[11]=0xd3;  

  dat1[12]=0x20;   //�ո����ASCII��Ϊ20H

  dat1[13]='U';
  dat1[14]='S';
  dat1[15]='B';
  dat1[16]='1';
  dat1[17]='1';
  dat1[18]='8';
  dat1[19]='A';

  	
  USB118_Write(dat1, 19);
  Del(70);
 // if(Command_status==0x01)
	if((commd[0]==0x0C)&(commd[1]==0x01)) 
	  {
	   break;
	   }  
 }
 Del(10);
 while(1);
 }

/*---------------------------------------------------
                �����жϺ���
�������ܣ���������
���ڲ�����Command_status��״̬
----------------------------------------------------*/
void serial() interrupt 4 using 0
{
  uint m;
  if(RI)
   {
   ES=0;
   m= Receive_One(commd, 0);
  //m=serial_rec(commd,0, 0);  
   if(m)
     Command_status=0x01;  	  	
  }
 }  