#include "STC15F2K60S2.H"
#define uint unsigned int
#define uchar unsigned char
/*���岿��*/
sbit beep=P3^4;//������
char beepflag; //��������־λ
uchar flag;//�����л���־λ
uchar flag1; //����ʱ��ʱ����Ϊʱ�����ѡ��λ
uchar dateflag;
bit stopflag;
bit resetflag;
uchar ring;//���ӿ��ر�־λ
uint  num;
uchar sec;
uchar min;
uchar hour;//ʱ����
uint  num1;
uchar sec1;
uchar min1;//���֡��롢����
uchar clocksec;
uchar clockmin;
uchar clockhour;//��������ʱ��
uchar day;
uchar month;
uint year;//������
uchar daynum;//ÿ�µ�����
uchar duanxuan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; //��������ܱ��� 0--9��- 
uchar weixuan[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07}; 
uchar dispbuf[8]={0,0,0,0,0,0,0,0};
uint tcnt=0;
/*���ݳ�ʼ������VarInit*/

void VarInit(void) 
{
	P2M0=0xff;
  	P2M1=0x00;
 	P0M0=0xff;
  	P0M1=0x00;//P0��P2������Ϊ�������

	num= 0;
	sec= 0;
	min= 0;
	hour= 12;//��ʼ��ʱ��

	num1=0; 
	sec1=0; 
	min1=0;//��ʼ�����
	
	clocksec=0; 
	clockmin=0; 
	clockhour=0;//��ʼ������ʱ��

	day= 10;
	month=9; 
	year=2017;//��ʼ������

	flag=0;
	flag1 =0;
	dateflag= 1; 
	stopflag=0;
	resetflag=0;
	ring=0;
}

/*��ʱ*/

void Delay_ms(uint xms)
{
	uchar j,k;
	for(j=xms;j>0;j--)
		for(k= 123;k>0;k--);
}

/*��ʱ����ʼ��*/

void TimerInit(void) 
{
	TMOD=0x02;
 	TH0=0x09; //װ��߰�λ��
 	TL0=0x66;//�Ͱ�λ
	TR0=1;//��������ʼ����
 	ET0=1;//��ʱ�������жϿ��� 
	EX0=1;//�ⲿ����0����λ 
 	EA=1;//�����ж�
}

/*��ʱ���жϺ���*/

void Time1() interrupt 1 using 0 
{
	tcnt++;
	if(tcnt==40)
	{
			tcnt=0;
			num++;
			if(num==100)
			{
				num=0;
				sec++;
				dateflag= 1 ;
			}
			if(sec==60)
			{
				sec= 0;
				min++;
			}
			if(min==60)
			{
				min= 0;
				hour++;
			}
			if(hour==24)
			{
				hour= 0;
			}
			if(stopflag== 1)
			{
					num1++;
					if(num1==100)
					{
						num1=0;
						sec1++;
					}
					if(sec1==60)
					{
						sec1 = 0;
						min1 ++;
					}
					if(min1==60)
					{
						min1 = 0;
					}
			}
	}
}

/*����ʱ��*/ 

void SetTime()
{
	if(flag==1)
	{
		if(P17==0)
		{
			Delay_ms(10);
			if(P17==0)
			{
				while(!P17);
				Delay_ms(10);
				while(!P17);
				flag1++;
				if(flag1==3)
				{
					flag1=0;
					sec=0;
				}
			}
		}
		switch(flag1)
		{
			case 0:
			if(P33==0) 
			{
				Delay_ms(10);
				if(P33==0)
				{
					while(!P33);
					Delay_ms(10);
					while(!P33);
					sec++;
					if(sec==60)
						sec=0;
				}
			}
			break;
			case 1:
			if(P33==0)
			{
				Delay_ms(10);
				if(P33==0)
				{
					while(!P33);
					Delay_ms(10);
					while(!P33);
					min++;
					if(min==60)
						min=0;
				}
			}
			break;
			case 2:
			if(P33==0)
			{
				Delay_ms(10);
				if(P33==0)
				{
					while(!P33);
					Delay_ms(10);
					while(!P33);
					hour++;
					if(hour==24)
						hour=0;
				}
			}
			break;

		}
	}
}

/*��������*/

void AlarmClock() 
{
	if(flag==2)
	{
		if(P17==0)
		{
			Delay_ms(10);
			if(P17==0)
			{
				while(!P17);
				Delay_ms(10);
				while(!P17);
				flag1++;
				if(flag1==3)
					flag1=0;
			}
		}
		switch (flag1)
		{
			case 0:
				if(P33==0)
				{
					Delay_ms(10);
					if(P33==0)
					{
						while(!P33);
						Delay_ms(10);
						while(!P33);
						clocksec++;
						if(clocksec==60)
							clocksec=0;
					}
				}
				break;
			case 1:
				if(P33==0)
				{
					Delay_ms(10);
					if(P33==0)
					{
						while(!P33);
						Delay_ms(10);
						while(!P33);
						clockmin++;
						if(clockmin==60)
							clockmin=0;
					}
				}
				break;
			case 2:
				if(P33==0)
				{
					Delay_ms(10);
					if(P33==0)
					{
						while( !P33);
						Delay_ms(10);
						while(!P33);
						clockhour++;
						if(clockhour==24)
							clockhour=0;
					}
				}
				break;
		}
	}

		if((clocksec==sec)&&(clockmin==min)&&(clockhour==hour))
		{
			beepflag=0;
			beep=0;
			Delay_ms( 1);
			beep= 1 ;
			Delay_ms( 1);
			beepflag++;
		}
	else
	{
		beep=1;
		beepflag=0;
	}
	if(beepflag==1)
	{
		beep=0;
		Delay_ms( 1);
		beep= 1 ;
		Delay_ms( 1);
	}
}

/*��������*/

void SetDate()
{
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12: daynum= 31;
					break;
		case 4:
		case 6:
		case 9:
		case 11: daynum= 30;
					break;
		case 2:
			if((((year%4)==0&&(year% 100) !=0)) || (year% 100)==0&&(year%400)==0)
			{
				daynum= 29;
			} 
			else 
			{
				daynum= 28;
			}
			break;
	}
	if(flag==3)
	{
		if(P17==0)
		{
			Delay_ms(10);
			if(P17==0)
			{
				while(!P17);
				Delay_ms(10);
				while(!P17);
				flag1++;
				if(flag1==3)
				{
					flag1=0;
				}
			}
		}
		switch (flag1)
		{
			case 0:
				if(P33==0) 
				{
					Delay_ms(10); 
					if(P33==0)
					{
						while(!P33);
						Delay_ms( 10);
						while(!P33);
						day++;
						if(day>=(daynum+ 1 ))
							day= 1 ;

					}
				}
				break;
			case 1:
				if(P33==0)
				{
					Delay_ms(10);
					if(P33==0)
					{
						while(!P33);
						Delay_ms(10);
						while(!P33);
						month++;
						if(month>= 13)
							month= 1;
					}
				}
				break;
			case 2:
				if(P33==0)
				{
					Delay_ms(10);
					if(P33==0)
					{
						while(!P33);
						Delay_ms(10);
						while(!P33);
						year++;
						if(year==3000)
							year=2017;
					}
				}
				break;
		}
	}
	if((hour==23)&&(min==59)&&(sec==59)&&(dateflag== 1)) 
	{
		dateflag=0;
		day++;
		if(day==(daynum+1)) 
		{
			day=1;
			month++;
			if(month== 13)
			{
				month= 1 ;
				year++;
			}
		}
	}
}

/*������*/ 

void StopWatch() 
{
if (flag==4)
{	
	if(P17==0)
	{
		Delay_ms(10);
		if(P17==0)
		{
			while(!P17);
			Delay_ms( 10);
			while(!P17);
			stopflag=~stopflag;
		}
	}
	if(P33==0)
	{
		Delay_ms(10);
		if(P33==0)
		{
			while(!P33);
			Delay_ms(10);
			while(!P33);
			resetflag=~resetflag;
			if(resetflag== 1 )
			{
				resetflag=0;
				stopflag=0;
				num1=0;
				sec1=0;
				min1=0;
			}
		}
	}
}
}

/*��ʾ����*/

void Display()
{
	if(flag==0||flag== 1)
	{
		uchar wei_1;
		uchar wei_2;
		dispbuf[2]=10;
		dispbuf[5]=10;
		num1=0; 
		sec1=0; 
		min1=0;//��ʼ�����
		wei_1 =hour/10;
		wei_2 = hour%10;
		dispbuf[0]=wei_1;
		dispbuf[1]=wei_2;
		wei_1 =min/10; 
		wei_2 = min%10; 
		dispbuf[3]=wei_1;
		dispbuf[4]=wei_2;
		wei_1 = sec/10; 
		wei_2 = sec%10; 
		dispbuf[6]=wei_1;
		dispbuf[7]=wei_2;
		P0=duanxuan[dispbuf[0]];
 		P2=weixuan[0];
		Delay_ms(10);
		P0=duanxuan[dispbuf[1]];
 		P2=weixuan[1];
		Delay_ms(10);
		P0=duanxuan[dispbuf[2]];
 		P2=weixuan[2];
		Delay_ms(10);
		P0=duanxuan[dispbuf[3]];
 		P2=weixuan[3];
		Delay_ms(10);
		P0=duanxuan[dispbuf[4]];
 		P2=weixuan[4];
		Delay_ms(10);
		P0=duanxuan[dispbuf[5]];
 		P2=weixuan[5];
		Delay_ms(10);
		P0=duanxuan[dispbuf[6]];
 		P2=weixuan[6];
		Delay_ms(10);
		P0=duanxuan[dispbuf[7]];
 		P2=weixuan[7];
		Delay_ms(10);
	}
	if(flag==2)
	{
		uchar wei_1;
		uchar wei_2;
		dispbuf[2]=10;
		dispbuf[5]=10;
		wei_1 =clockhour/10;//��ʾСʱ
		wei_2 = clockhour%10;
		dispbuf[0]=wei_1;
		dispbuf[1]=wei_2;
		wei_1 =clockmin/10;//��ʾ����
		wei_2 = clockmin%10;
		dispbuf[3]=wei_1;
		dispbuf[4]=wei_2;
		wei_1 = clocksec/10;
		wei_2 = clocksec%10; 
		dispbuf[6]=wei_1;
		dispbuf[7]=wei_2;
		P0=duanxuan[dispbuf[0]];
 		P2=weixuan[0];
		Delay_ms(10);
		P0=duanxuan[dispbuf[1]];
 		P2=weixuan[1];
		Delay_ms(10);
		P0=duanxuan[dispbuf[2]];
 		P2=weixuan[2];
		Delay_ms(10);
		P0=duanxuan[dispbuf[3]];
 		P2=weixuan[3];
		Delay_ms(10);
		P0=duanxuan[dispbuf[4]];
 		P2=weixuan[4];
		Delay_ms(10);
		P0=duanxuan[dispbuf[5]];
 		P2=weixuan[5];
		Delay_ms(10);
		P0=duanxuan[dispbuf[6]];
 		P2=weixuan[6];
		Delay_ms(10);
		P0=duanxuan[dispbuf[7]];
 		P2=weixuan[7];
		Delay_ms(10);
	}
	if(flag==3)
	{
		uchar wei_1;
		uchar wei_2;
		dispbuf[2]=10;
		dispbuf[5]=10;
		wei_1 =(year%100)/10;    //��ʾ��
		wei_2 = year% 10;
		dispbuf[0]=wei_1;
		dispbuf[1]=wei_2;
		wei_1 =month/10;      //��ʾ��
		wei_2 = month%10;
		dispbuf[3]=wei_1;
		dispbuf[4]=wei_2;
		wei_1 =day/10;//��ʾ�� 
		wei_2 = day%10; 
		dispbuf[6]=wei_1;
		dispbuf[7]=wei_2;
		P0=duanxuan[dispbuf[0]];
 		P2=weixuan[0];
		Delay_ms(10);
		P0=duanxuan[dispbuf[1]];
 		P2=weixuan[1];
		Delay_ms(10);
		P0=duanxuan[dispbuf[2]];
 		P2=weixuan[2];
		Delay_ms(10);
		P0=duanxuan[dispbuf[3]];
 		P2=weixuan[3];
		Delay_ms(10);
		P0=duanxuan[dispbuf[4]];
 		P2=weixuan[4];
		Delay_ms(10);
		P0=duanxuan[dispbuf[5]];
 		P2=weixuan[5];
		Delay_ms(10);
		P0=duanxuan[dispbuf[6]];
 		P2=weixuan[6];
		Delay_ms(10);
		P0=duanxuan[dispbuf[7]];
 		P2=weixuan[7];
		Delay_ms(10);
	}
	if(flag==4)
	{
		uchar wei_1;
		uchar wei_2;
		wei_1 = (min1%100)/10; //��ʾ���� 
		wei_2 = min1%10;
		dispbuf[0]=wei_1;
		dispbuf[1]=wei_2;
		wei_1 =sec1/10; //��ʾ�� 
		wei_2 = sec1%10; 
		dispbuf[3]=wei_1;
		dispbuf[4]=wei_2;
		wei_1 =num1/10;//��ʾ���뼶
		wei_2 = num1%10; 
		dispbuf[6]=wei_1;
		dispbuf[7]=wei_2;
		P0=duanxuan[dispbuf[0]];
 		P2=weixuan[0];
		Delay_ms(10);
		P0=duanxuan[dispbuf[1]];
 		P2=weixuan[1];
		Delay_ms(10);
		P0=duanxuan[dispbuf[2]];
 		P2=weixuan[2];
		Delay_ms(10);
		P0=duanxuan[dispbuf[3]];
 		P2=weixuan[3];
		Delay_ms(10);
		P0=duanxuan[dispbuf[4]];
 		P2=weixuan[4];
		Delay_ms(10);
		P0=duanxuan[dispbuf[5]];
 		P2=weixuan[5];
		Delay_ms(10);
		P0=duanxuan[dispbuf[6]];
 		P2=weixuan[6];
		Delay_ms(10);
		P0=duanxuan[dispbuf[7]];
 		P2=weixuan[7];
		Delay_ms(10);
	}
}
void main()
{
	unsigned char i,j;
	VarInit();
	TimerInit();
	while( 1)
	{
		if(P32==0)
 		{
 			for(i=5;i>0;i--)
 				for(j=248;j>0;j--);
 			if(P32==0)
 			{
				flag++;
				if (flag==5)
					flag=0;
 				while(!P32);
				for(i=5;i>0;i--)
					for(j=248;j>0;j--);
				while(!P32);
 			}
 		}
		SetTime();
		AlarmClock(); 
		SetDate();
		StopWatch(); 
		Display();
	}
}
