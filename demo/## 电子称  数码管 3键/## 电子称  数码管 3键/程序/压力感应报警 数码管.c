#include <reg52.h>	         //调用单片机头文件
#define uchar unsigned char  //无符号字符型 宏定义	变量范围0~255
#define uint  unsigned int	 //无符号整型 宏定义	变量范围0~65535
#include <intrins.h>

//数码管段选定义      0     1    2    3    4    5	6	 7	  8	   9	
uchar code smg_du[]={0x28,0xe1,0x32,0xa2,0x54,0xa1,0x21,0xea,0x20,0xa0,
				  	 0x60,0x25,0x39,0x26,0x31,0x71,0xff};	 //断码
//数码管位选定义
uchar code smg_we[]={0xef,0xdf,0xbf,0x7f};
uchar dis_smg[8] = {0};	
uchar smg_i = 4;    //显示数码管的个位数
sbit beep = P2^3;   //蜂鸣器IO口定义

bit flag_lj_en;		 //按键连加使能
bit flag_lj_3_en;	 //按键连3次连加后使能  加的数就越大了 
uchar key_time,key_value;      //用做连加的中间变量
bit key_500ms  ;

bit flag_300ms ;
uchar menu_1;       //菜单设计的变量
uint t_high = 300;	   //上限报警值 

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

/***********************1ms延时函数*****************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<120;j++);
}

/***********************数码显示函数*****************************/
void display()
{
	static uchar i;   
	i++;
	if(i >= smg_i)
		i = 0;	
	P1 = 0xff;			 //消隐 
	P3 = smg_we[i];	 		 //位选
	P1 = dis_smg[i];		 //段选	        
}


/*************定时器0初始化程序***************/
void time_init()	  
{
	EA   = 1;	 	  //开总中断
	TMOD = 0X01;	  //定时器0、定时器1工作方式1
	ET0  = 1;		  //开定时器0中断 
	TR0  = 1;		  //允许定时器0定时
}

/********************独立按键程序*****************/
uchar key_can;	 //按键值

void key()	 //独立按键程序
{
	static uchar key_new;
	key_can = 20;                   //按键值还原
	P2 |= 0x07;
	if((P2 & 0x07) != 0x07)		//按键按下
	{
		if(key_500ms == 1)	//连加
		{
			key_500ms = 0;
			key_new = 1;
		}
		delay_1ms(1);	     	//按键消抖动
		if(((P2 & 0x07) != 0x07) && (key_new == 1))
		{						//确认是按键按下
			key_new = 0;
			switch(P2 & 0x07)
			{
				case 0x06: key_can = 3; break;	   //得到k2键值
				case 0x05: key_can = 2; break;	   //得到k3键值
				case 0x03: key_can = 1; break;	   //得到k4键值
			}
			flag_lj_en = 1;	 //连加使能
		}			
	}
	else 
	{
		if(key_new == 0)
		{
			key_new = 1;
			flag_lj_en = 0;		//关闭连加使能
			flag_lj_3_en = 0;	//关闭3秒后使能
			key_value = 0;		//清零
			key_time = 0;
			key_500ms = 0;
		}
	}	
}

/****************按键处理数码管显示函数***************/
void key_with()
{
	if(key_can == 1)	  //设置键
	{
		menu_1 ++;
		if(menu_1 >= 2)
		{
			menu_1 = 0;
			smg_i = 4;		  //数码管显示3位
			flag_300ms = 1;
		}
	}
	if(menu_1 == 1)			//设置报警
	{
		smg_i = 4;		  //数码管显示4位
		if(key_can == 2)
		{
			if(flag_lj_3_en == 0)
				t_high += 10 ;		//按键按下未松开自动加三次	
			else 
				t_high += 100;	//按键按下未松开自动加三次之后每次自动加10
			if(t_high > 9990)
				t_high = 9990;
		}
		if(key_can == 3)
		{
			if(flag_lj_3_en == 0)
				t_high -= 10 ;		//按键按下未松开自动减三次	
			else 
				t_high -= 100;	//按键按下未松开自动减三次之后每次自动减10
			if(t_high <= 10)
				t_high = 10;
		}
		dis_smg[0] = smg_du[t_high / 10 % 10];	           //取小数显示
		dis_smg[1] = smg_du[t_high / 100 % 10] ;  //取个位显示
		dis_smg[2] = smg_du[t_high / 1000 % 10] & 0xdf;	   //取十位显示
		dis_smg[3] = 0x64;	 //H
	}	
}  

/****************报警函数***************/
void clock_h_l()
{
	static uchar value;
	if(weight >= t_high)
	{
		value ++;  //消除实际距离在设定距离左右变化时的干扰
		if(value >= 2)
		{
			beep = ~beep; 	  //蜂鸣器报警			
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

unsigned long HX711_Read(void)	//增益128
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
    count=count^0x8000;//下降沿来时，转换数据
	Delay__hx711_us();
	hx711_sck=0;  
	return(count);
}

void get_weight()	  //获取被测物体重量
{
	weight = HX711_Read();
	weight = weight - Weight_Maopi;		//获取净重
	if(weight > 0)			
	{	
		weight = (unsigned int)((float)weight/GapValue); 	//计算实物的实际重量																	
	}
	else
	{
		weight = 0;
	}
}


/****************主函数***************/
void main()
{
	time_init();                    //初始化定时器 
	while(1)
	{		
		key();					//按键程序
		if(key_can < 10)
		{
			key_with();			//设置报警压力	
		}
		if(flag_300ms == 1)	    //300ms 处理一次压力程序
		{	   
			flag_300ms = 0;	
			get_weight();	  //获取被测物体重量
			clock_h_l();        //报警函数
			if(menu_1 == 0)
			{	
				smg_i = 4;
				dis_smg[0] = smg_du[weight % 10];	 //取压力的小数显示
				dis_smg[1] = smg_du[weight / 10 % 10] ; //取压力的个位显示
				dis_smg[2] = smg_du[weight / 100 % 10] ;	   //取压力的十位显示
				dis_smg[3] = smg_du[weight / 1000 % 10] & 0xdf;	   //取压力的百位显示
			}
		}
		display();		//数码管显示函数
	}
}

/*************定时器0中断服务程序***************/
void time0_int() interrupt 1
{	
	static uchar value;			 //定时2ms中断一次
	TH0 = 0xf8;
	TL0 = 0x30;     //2ms
	value++;	  
	if(value >= 100)
	{
		value = 0;	  
		flag_300ms = 1;
	}
	if(flag_lj_en == 1)	   //按下按键使能
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
				flag_lj_3_en = 1; //3次后1.5秒连加大些
			}						
		}
	}
}