#include <reg52.h>	         //���õ�Ƭ��ͷ�ļ�
#define uchar unsigned char  //�޷����ַ��� �궨��	������Χ0~255
#define uint  unsigned int	 //�޷������� �궨��	������Χ0~65535
#include <intrins.h>

//����ܶ�ѡ����      0     1    2    3    4    5	6	 7	  8	   9	
uchar code smg_du[]={0x28,0xe1,0x32,0xa2,0x54,0xa1,0x21,0xea,0x20,0xa0,
				  	 0x60,0x25,0x39,0x26,0x31,0x71,0xff};	 //����
//�����λѡ����
uchar code smg_we[]={0xef,0xdf,0xbf,0x7f};
uchar dis_smg[8] = {0};	
uchar smg_i = 4;    //��ʾ����ܵĸ�λ��
sbit beep = P2^3;   //������IO�ڶ���

bit flag_lj_en;		 //��������ʹ��
bit flag_lj_3_en;	 //������3�����Ӻ�ʹ��  �ӵ�����Խ���� 
uchar key_time,key_value;      //�������ӵ��м����
bit key_500ms  ;

bit flag_300ms ;
uchar menu_1;       //�˵���Ƶı���
uint t_high = 300;	   //���ޱ���ֵ 

sbit hx711_dout=P2^7; 
sbit hx711_sck=P2^6; 

long weight;

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;

uint temp,qi_weight;
bit chaozhong_flag;
bit leiji_flag;
uchar flag_200ms ;

#define GapValue 437.0

uint temp,qi_weight;
bit chaozhong_flag;
bit leiji_flag;

/***********************1ms��ʱ����*****************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<120;j++);
}

/***********************������ʾ����*****************************/
void display()
{
	static uchar i;   
	i++;
	if(i >= smg_i)
		i = 0;	
	P1 = 0xff;			 //���� 
	P3 = smg_we[i];	 		 //λѡ
	P1 = dis_smg[i];		 //��ѡ	        
}


/*************��ʱ��0��ʼ������***************/
void time_init()	  
{
	EA   = 1;	 	  //�����ж�
	TMOD = 0X01;	  //��ʱ��0����ʱ��1������ʽ1
	ET0  = 1;		  //����ʱ��0�ж� 
	TR0  = 1;		  //����ʱ��0��ʱ
}

/********************������������*****************/
uchar key_can;	 //����ֵ

void key()	 //������������
{
	static uchar key_new;
	key_can = 20;                   //����ֵ��ԭ
	P2 |= 0x07;
	if((P2 & 0x07) != 0x07)		//��������
	{
		if(key_500ms == 1)	//����
		{
			key_500ms = 0;
			key_new = 1;
		}
		delay_1ms(1);	     	//����������
		if(((P2 & 0x07) != 0x07) && (key_new == 1))
		{						//ȷ���ǰ�������
			key_new = 0;
			switch(P2 & 0x07)
			{
				case 0x06: key_can = 3; break;	   //�õ�k2��ֵ
				case 0x05: key_can = 2; break;	   //�õ�k3��ֵ
				case 0x03: key_can = 1; break;	   //�õ�k4��ֵ
			}
			flag_lj_en = 1;	 //����ʹ��
		}			
	}
	else 
	{
		if(key_new == 0)
		{
			key_new = 1;
			flag_lj_en = 0;		//�ر�����ʹ��
			flag_lj_3_en = 0;	//�ر�3���ʹ��
			key_value = 0;		//����
			key_time = 0;
			key_500ms = 0;
		}
	}	
}

/****************���������������ʾ����***************/
void key_with()
{
	if(key_can == 1)	  //���ü�
	{
		menu_1 ++;
		if(menu_1 >= 2)
		{
			menu_1 = 0;
			smg_i = 4;		  //�������ʾ3λ
			flag_300ms = 1;
		}
	}
	if(menu_1 == 1)			//���ñ���
	{
		smg_i = 4;		  //�������ʾ4λ
		if(key_can == 2)
		{
			if(flag_lj_3_en == 0)
				t_high += 10 ;		//��������δ�ɿ��Զ�������	
			else 
				t_high += 100;	//��������δ�ɿ��Զ�������֮��ÿ���Զ���10
			if(t_high > 9990)
				t_high = 9990;
		}
		if(key_can == 3)
		{
			if(flag_lj_3_en == 0)
				t_high -= 10 ;		//��������δ�ɿ��Զ�������	
			else 
				t_high -= 100;	//��������δ�ɿ��Զ�������֮��ÿ���Զ���10
			if(t_high <= 10)
				t_high = 10;
		}
		dis_smg[0] = smg_du[t_high / 10 % 10];	           //ȡС����ʾ
		dis_smg[1] = smg_du[t_high / 100 % 10] ;  //ȡ��λ��ʾ
		dis_smg[2] = smg_du[t_high / 1000 % 10] & 0xdf;	   //ȡʮλ��ʾ
		dis_smg[3] = 0x64;	 //H
	}	
}  

/****************��������***************/
void clock_h_l()
{
	static uchar value;
	if(weight >= t_high)
	{
		value ++;  //����ʵ�ʾ������趨�������ұ仯ʱ�ĸ���
		if(value >= 2)
		{
			beep = ~beep; 	  //����������			
		}	
	}
	else
	{
		beep = 1;	
	}			
}


void Delay__hx711_us(void)
{
	_nop_();
	_nop_();
}

unsigned long HX711_Read(void)	//����128
{
	unsigned long count; 
	unsigned char i; 
  	hx711_sck=0; 
	Delay__hx711_us();
  	hx711_dout=1; 
  	while(hx711_dout==0); 
  	for(i=0;i<24;i++)
	{ 
		hx711_sck=0; 
	  	count=count<<1; 
	  	hx711_sck=1; 
	  	if(hx711_dout)
			count++; 
	} 
 	hx711_sck=1; 
    count=count^0x8000;//�½�����ʱ��ת������
	Delay__hx711_us();
	hx711_sck=0;  
	return(count);
}

void get_weight()	  //��ȡ������������
{
	weight = HX711_Read();
	weight = weight - Weight_Maopi;		//��ȡ����
	if(weight > 0)			
	{	
		weight = (unsigned int)((float)weight/GapValue); 	//����ʵ���ʵ������																	
	}
	else
	{
		weight = 0;
	}
}


/****************������***************/
void main()
{
	time_init();                    //��ʼ����ʱ�� 
	while(1)
	{		
		key();					//��������
		if(key_can < 10)
		{
			key_with();			//���ñ���ѹ��	
		}
		if(flag_300ms == 1)	    //300ms ����һ��ѹ������
		{	   
			flag_300ms = 0;	
			get_weight();	  //��ȡ������������
			clock_h_l();        //��������
			if(menu_1 == 0)
			{	
				smg_i = 4;
				dis_smg[0] = smg_du[weight % 10];	 //ȡѹ����С����ʾ
				dis_smg[1] = smg_du[weight / 10 % 10] ; //ȡѹ���ĸ�λ��ʾ
				dis_smg[2] = smg_du[weight / 100 % 10] ;	   //ȡѹ����ʮλ��ʾ
				dis_smg[3] = smg_du[weight / 1000 % 10] & 0xdf;	   //ȡѹ���İ�λ��ʾ
			}
		}
		display();		//�������ʾ����
	}
}

/*************��ʱ��0�жϷ������***************/
void time0_int() interrupt 1
{	
	static uchar value;			 //��ʱ2ms�ж�һ��
	TH0 = 0xf8;
	TL0 = 0x30;     //2ms
	value++;	  
	if(value >= 100)
	{
		value = 0;	  
		flag_300ms = 1;
	}
	if(flag_lj_en == 1)	   //���°���ʹ��
	{
		key_time ++;
		if(key_time >= 250) //500ms
		{
			key_time = 0;
			key_500ms = 1; //500ms
			key_value ++;
			if(key_value > 3)
			{
				key_value = 10;
				flag_lj_3_en = 1; //3�κ�1.5�����Ӵ�Щ
			}						
		}
	}
}