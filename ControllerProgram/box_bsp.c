#include "box_bsp.h"
#include <string.h>
#define BUFFER_SIZE 16
#define STATE_BUFFER_SIZE 3

char busy;
unsigned char buffer[BUFFER_SIZE];

unsigned char rec_byte;
unsigned char led_state;
unsigned char Dev_address;
unsigned char lock_vector;
char j;
unsigned char state_buffer[STATE_BUFFER_SIZE];
int open_lock_timer = -1;
int key_set_on_time = -1;
int key_open_on_time = -1;

void Timer0_Init(void) // 1毫秒@12MHz
{
	AUXR |= 0x80; // 定时器时钟1T模式
	TMOD &= 0xF0; // 设置定时器模式
	TL0 = 0x20;	  // 设置定时初始值
	TH0 = 0xD1;	  // 设置定时初始值
	TF0 = 0;	  // 清除TF0标志
	TR0 = 1;	  // 定时器0开始计时
	ET0 = 1;
}

void TM0_Isr() interrupt 1
{
	if (key_open_on_time >= 0)
	{
		key_open_on_time++;
	}
	if (key_set_on_time >= 0)
	{
		key_set_on_time++;
	}
	if (open_lock_timer >= 0)
	{
		open_lock_timer++;
	}
}

void led_set(unsigned char state)
{
	unsigned char led_state = ~state;
	LED_GROUP_0 = led_state & 0x01;
	LED_GROUP_1 = led_state & 0x02;
	LED_GROUP_2 = led_state & 0x04;
	LED_GROUP_3 = led_state & 0x08;
	LED_GROUP_4 = led_state & 0x10;
}

unsigned char getKeyGroupState()
{
	unsigned char temp = 0x00;
	temp |= KEY_GROUP_0 ? 0x01 : 0x00;
	temp |= KEY_GROUP_1 ? 0x02 : 0x00;
	temp |= KEY_GROUP_2 ? 0x04 : 0x00;
	temp |= KEY_GROUP_3 ? 0x08 : 0x00;
	temp |= KEY_GROUP_4 ? 0x10 : 0x00;
	return ((~temp) & 0x1F);
}

void UartInit(void) // 9600bps@12.000MHz
{
	SCON = 0x50;  // 8位数据,可变波特率
	AUXR |= 0x40; // 定时器时钟1T模式
	AUXR &= 0xFE; // 串口1选择定时器1为波特率发生器
	TMOD &= 0x0F; // 设置定时器模式
	TL1 = 0xC7;	  // 设置定时初始值
	TH1 = 0xFE;	  // 设置定时初始值
	ET1 = 0;	  // 禁止定时器中断
	TR1 = 1;	  // 定时器1开始计时
}

void GPIO_init()
{

	CHW_0 = 0;
	CHW_1 = 0;
	CHW_2 = 0;
	CHW_3 = 0;
	CHW_4 = 0;
	CHW_5 = 0;
	CHW_6 = 0;
	CHW_7 = 0;
	CHW_8 = 0;
	CHW_9 = 0;
	CHW_10 = 0;
	CHW_11 = 0;
	CHW_12 = 0;
	CHW_13 = 0;
	CHW_14 = 0;
	CHW_15 = 0;
	CHW_16 = 0;
	CHW_17 = 0;

	CHR_0 = 1;
	CHR_1 = 1;
	CHR_2 = 1;
	CHR_3 = 1;
	CHR_4 = 1;
	CHR_5 = 1;
	CHR_6 = 1;
	CHR_7 = 1;
	CHR_8 = 1;
	CHR_9 = 1;
	CHR_10 = 1;
	CHR_11 = 1;
	CHR_12 = 1;
	CHR_13 = 1;
	CHR_14 = 1;
	CHR_15 = 1;
	CHR_16 = 1;
	CHR_17 = 1;

	KEY_GROUP_0 = 1;
	KEY_GROUP_1 = 1;
	KEY_GROUP_2 = 1;
	KEY_GROUP_3 = 1;
	KEY_GROUP_4 = 1;
	KEY_OPEN = 1;
	KEY_SET = 1;

	SYS_LED = 0;
	RS485_RX;




	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;

	P3M0 = 0x00;
	P3M1 = 0x00;

	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	P6M0 = 0x00;
	P6M1 = 0x00;
	P7M0 = 0x00;
	P7M1 = 0x00;


}

void Drain_set(unsigned char gpio_pin, unsigned char set)
{
	switch (gpio_pin)
	{
	case 0x00:
		CHW_0 = set;
		break;
	case 0x01:
		CHW_1 = set;
		break;
	case 0x02:
		CHW_2 = set;
		break;
	case 0x03:
		CHW_3 = set;
		break;
	case 0x04:
		CHW_4 = set;
		break;
	case 0x05:
		CHW_5 = set;
		break;
	case 0x06:
		CHW_6 = set;
		break;
	case 0x07:
		CHW_7 = set;
		break;
	case 0x08:
		CHW_8 = set;
		break;
	case 0x09:
		CHW_9 = set;
		break;
	case 0x0A:
		CHW_10 = set;
		break;
	case 0x0B:
		CHW_11 = set;
		break;
	case 0x0C:
		CHW_12 = set;
		break;
	case 0x0D:
		CHW_13 = set;
		break;
	case 0x0E:
		CHW_14 = set;
		break;
	case 0x0F:
		CHW_15 = set;
		break;
	case 0x10:
		CHW_16 = set;
		break;
	case 0x11:
		CHW_17 = set;
		break;
	default:

		break;
	}
}

unsigned char feedback_read(char gpio_pin)
{
	unsigned char temp;
	switch (gpio_pin)
	{
	case 0x00:
		temp = CHR_0;
		break;
	case 0x01:
		temp = CHR_1;
		break;
	case 0x02:
		temp = CHR_2;
		break;
	case 0x03:
		temp = CHR_3;
		break;
	case 0x04:
		temp = CHR_4;
		break;
	case 0x05:
		temp = CHR_5;
		break;
	case 0x06:
		temp = CHR_6;
		break;
	case 0x07:
		temp = CHR_7;
		break;
	case 0x08:
		temp = CHR_8;
		break;
	case 0x09:
		temp = CHR_9;
		break;
	case 0x0A:
		temp = CHR_10;
		break;
	case 0x0B:
		temp = CHR_11;
		break;
	case 0x0C:
		temp = CHR_12;
		break;
	case 0x0D:
		temp = CHR_13;
		break;
	case 0x0E:
		temp = CHR_14;
		break;
	case 0x0F:
		temp = CHR_15;
		break;
	case 0x10:
		temp = CHR_16;
		break;
	case 0x11:
		temp = CHR_17;
		break;
	default:
			temp = 0x01;
	}

	if (lock_vector==0x01)
	{
		temp = !temp;
	}
	return temp;
}

void get_state_buffer()
{

	char i, j, temp;
	for (i = 0; i < STATE_BUFFER_SIZE; i++)
	{
		temp = 0x00;
		for (j = 0; j < 8; j++)
		{
			temp |= (feedback_read(i * 8 + j)) << j;
		}
		state_buffer[i] = temp;
	}
}

void put_stack(char dat)
{
	char i;
	for (i = 0; i < BUFFER_SIZE - 1; i++)
		buffer[i] = buffer[i + 1];
	buffer[BUFFER_SIZE - 1] = dat;
}

void UART1_Isr() interrupt 4
{
	if (TI) // 发送中断标志位判断
	{
		TI = 0;				// 清中断标志
		busy = 0;			// 发送标志位置0，表示满足发送条件
		SYS_LED = !SYS_LED; // 测试端口
	}
	if (RI) // 接收中断标志位判断
	{
		RI = 0; // 清中断标志

		SYS_LED = !SYS_LED; // 测试端口
		rec_byte = SBUF;

		put_stack(rec_byte);
	}
}

void UartSend(char dat)
{
	while (busy)
		;
	busy = 1;
	SBUF = dat;
}

void BOX_Init()
{
	GPIO_init();
	UartInit();
	Timer0_Init();
	P_SW2 |= 0x80;
	ES = 1;
	EA = 1;
	delay_1s();

	Dev_address = IapRead(0x0000);
	lock_vector = IapRead(0x0001);
	if (Dev_address == 0xFF&&lock_vector==0xFF)
	{
		IapErase(0x0000);
		IapProgram(0x0000, 0x00);
		IapProgram(0x0001, 0x01);
		Dev_address = IapRead(0x0000);
		lock_vector = IapRead(0x0001);
	}



	WDT_CONTR = 0x27; // 使能看门狗,溢出时间约为1s

	while (1)
	{
		WDT_CONTR = 0x37;
		loop();
	}
}

void Delay1ms() //@12.000MHz
{
	unsigned char data i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j)
			;
	} while (--i);
}

void delay_1s()
{

	unsigned int i;
	for (i = 0; i < 1000; i++)
	{
		Delay1ms();
	}
}

void delay_500ms()
{

	unsigned int i;
	for (i = 0; i < 500; i++)
	{
		Delay1ms();
	}
}

void delay(int ms)
{
	unsigned int i;
	for (i = 0; i < ms; i++)
	{
		Delay1ms();
	}
}

void key_scan()
{

	//====================
	if (KEY_OPEN == 0)
		if (key_open_on_time == -1)
			key_open_on_time = 0;
	if (KEY_OPEN)
		if (key_open_on_time == -2)
			key_open_on_time = -1;
	if (key_open_on_time > 500)
	{
		key_open_on_time = -2;
		if (KEY_OPEN == 0)
			open_all_lock_key();
	}

	//====================
	if (KEY_SET == 0)
		if (key_set_on_time == -1)
			key_set_on_time = 0;
	if (KEY_SET)
		if (key_set_on_time == -2)
			key_set_on_time = -1;

	if (key_set_on_time > 3000)
	{
		key_set_on_time = -2;

		if (KEY_SET == 0)
		{
			IapErase(0x0000);
			IapProgram(0x0000, getKeyGroupState());
			IapProgram(0x0001, lock_vector);
			Dev_address = IapRead(0x0000);
			lock_vector=IapRead(0x0001);
			memset(buffer, 0, 16);
		}
	}
}

void loop()
{

	key_scan();
	led_set(Dev_address);
	cmd_process();
}

void cmd_process()
{
	unsigned char i;
	for (i = 0; i < BUFFER_SIZE; i++)
	{
		if (buffer[i] >= 0x80)
		{

			if (i < BUFFER_SIZE - 4)
				if (buffer[i + 1] == (Dev_address + 1))
					if (buffer[i + 4] == (buffer[i] ^ buffer[i + 1] ^ buffer[i + 2] ^ buffer[i + 3]))
					{
						RS485_TX;

						open_lock_cmd(i);
						open_all_lock_cmd(i);
						output_enable(i);
						output_disable(i);
						get_lock_state_cmd(i);
						get_chipid(i);
						get_version(i);
						set_vector( i);
						 get_vector(i);

						memset(buffer, 0, 16);

						while (busy)
							;
						RS485_RX;
					}
		}
	}
}

void open_lock_cmd(unsigned char i)
{
	if (buffer[i] != 0x8A)
		return;
	Drain_set(buffer[i + 2] - 1, 1);
	delay(400);
	Drain_set(buffer[i + 2] - 1, 0);
	delay(100);

	UartSend(0x8A);
	UartSend(buffer[i + 1]);
	UartSend(buffer[i + 2]);
	if (feedback_read(buffer[i + 2] - 1))
	{
		UartSend(0x11);
		UartSend(0x8A ^ buffer[i + 1] ^ buffer[i + 2] ^ 0x11);
	}
	else
	{
		UartSend(0x00);
		UartSend(0x8A ^ buffer[i + 1] ^ buffer[i + 2] ^ 0x00);
	}
}

void open_all_lock_key()
{
	for (j = 0; j < CH_MAX; j++)
	{
		// Drain_set(j, 1);
		// open_lock_timer=0;
		// while ((!feedback_read(j))&&open_lock_timer<500);
		// Drain_set(j, 0);
		// open_lock_timer=-1;
		Drain_set(j, 1);
		delay(300);
		Drain_set(j, 0);
		delay(50);
		WDT_CONTR = 0x37;
	}
}

void open_all_lock()
{
	for (j = 0; j < CH_MAX; j++)
	{
		// Drain_set(j, 1);
		// open_lock_timer=0;
		// while ((!feedback_read(j))&&open_lock_timer<500);
		// Drain_set(j, 0);
		// open_lock_timer=-1;
		Drain_set(j, 1);
		delay(300);
		Drain_set(j, 0);
		delay(50);
		WDT_CONTR = 0x37;
	}
}

void open_all_lock_cmd(unsigned char i)
{
	if (buffer[i] != 0x9D)
		return;

	open_all_lock();
	UartSend(0x9D);
	UartSend(buffer[i + 1]);
	get_state_buffer();
	UartSend(0x00);
	UartSend(state_buffer[2]);
	UartSend(state_buffer[1]);
	UartSend(state_buffer[0]);
	UartSend(0x9D ^ buffer[i + 1] ^ state_buffer[2] ^ state_buffer[1] ^ state_buffer[0]);
}

void output_enable(unsigned char i)
{
	if (buffer[i] != 0x9A)
		return;
	Drain_set(buffer[i + 2] - 1, 1);
	delay_500ms();
	UartSend(0x9A);
	UartSend(buffer[i + 1]);
	UartSend(buffer[i + 2]);
	if (feedback_read(buffer[i + 2] - 1))
	{
		UartSend(0x11);
		UartSend(0x9A ^ buffer[i + 1] ^ buffer[i + 2] ^ 0x11);
	}
	else
	{
		UartSend(0x00);
		UartSend(0x9A ^ buffer[i + 1] ^ buffer[i + 2] ^ 0x00);
	}
}

void output_disable(unsigned char i)
{
	if (buffer[i] != 0x9B)
		return;
	Drain_set(buffer[i + 2] - 1, 0);
	delay_500ms();
	UartSend(0x9B);
	UartSend(buffer[i + 1]);
	UartSend(buffer[i + 2]);
	if (feedback_read(buffer[i + 2] - 1))
	{
		UartSend(0x11);
		UartSend(0x9B ^ buffer[i + 1] ^ buffer[i + 2] ^ 0x11);
	}
	else
	{
		UartSend(0x00);
		UartSend(0x9B ^ buffer[i + 1] ^ buffer[i + 2] ^ 0x00);
	}
}

void get_lock_state_cmd(unsigned char i)
{
	if (buffer[i] != 0x80)
		return;
	Delay1ms();
	if (buffer[i + 2] == 0x00)
	{
		UartSend(0x80);
		UartSend(buffer[i + 1]);
		get_state_buffer();
		UartSend(state_buffer[2]);
		UartSend(state_buffer[1]);
		UartSend(state_buffer[0]);
		UartSend(0x33);
		UartSend(0x80 ^ buffer[i + 1] ^ state_buffer[2] ^ state_buffer[1] ^ state_buffer[0] ^ 0x33);
	}
	else
	{
		UartSend(0x80);
		UartSend(buffer[i + 1]);
		UartSend(buffer[i + 2]);
		if (feedback_read(buffer[i + 2] - 1))
		{
			UartSend(0x11);
			UartSend(0x80 ^ buffer[i + 1] ^ buffer[i + 2] ^ 0x11);
		}
		else
		{
			UartSend(0x00);
			UartSend(0x80 ^ buffer[i + 1] ^ buffer[i + 2] ^ 0x00);
		}
	}
}

void get_chipid(unsigned char i)
{

	if (buffer[i] != 0xDD)
		return;
	Delay1ms();
	Delay1ms();
	UartSend(0xDD);
	UartSend(buffer[i + 1]);
	UartSend(buffer[i + 2]);
	UartSend(CHIPID0);
	UartSend(CHIPID1);
	UartSend(CHIPID2);
	UartSend(CHIPID3);
	UartSend(CHIPID4);
	UartSend(CHIPID5);
	UartSend(CHIPID6);
	UartSend(0xDD^buffer[i + 1]^buffer[i + 2]^CHIPID0^CHIPID1^CHIPID2^CHIPID3^CHIPID4^CHIPID5^CHIPID6);

}

void get_version(unsigned char i)
{

	if (buffer[i] != 0xDE)
		return;
	Delay1ms();
	Delay1ms();
	UartSend(0xDE);
	UartSend(buffer[i + 1]);
	UartSend(VERSION_H);
	UartSend(VERSION_L);
	UartSend(0xDE^buffer[i + 1]^VERSION_H^VERSION_L);

}

void set_vector(unsigned char i)
{

	if (buffer[i] != 0xDF)
		return;
	Delay1ms();
	Delay1ms();

	IapErase(0x0000);
	IapProgram(0x0000, Dev_address);
	IapProgram(0x0001, buffer[i+3]);
	Dev_address = IapRead(0x0000);
    lock_vector =  IapRead(0x0001);

	UartSend(0xDF);
	UartSend(buffer[i + 1]);
	UartSend(0x11);
	UartSend(0x11);
	UartSend(0xDF^buffer[i + 1]^0x11^0x11);

}

void get_vector(unsigned char i)
{

	if (buffer[i] != 0xE1)
		return;
	Delay1ms();
	Delay1ms();
	UartSend(0xE1);
	UartSend(buffer[i + 1]);
	UartSend(buffer[i + 2]);
	UartSend(lock_vector);
	UartSend(0xE1^buffer[i + 1]^buffer[i + 2]^lock_vector);

}

void IapIdle()
{
	IAP_CONTR = 0;	  // 关闭IAP功能
	IAP_CMD = 0;	  // 清除命令寄存器
	IAP_TRIG = 0;	  // 清除触发寄存器
	IAP_ADDRH = 0x80; // 将地址设置到非IAP区域
	IAP_ADDRL = 0;
}

char IapRead(int addr)
{
	char dat;

	IAP_CONTR = 0x80;	   // 使能IAP
	IAP_TPS = 12;		   // 设置等待参数12MHz
	IAP_CMD = 1;		   // 设置IAP读命令
	IAP_ADDRL = addr;	   // 设置IAP低地址
	IAP_ADDRH = addr >> 8; // 设置IAP高地址
	IAP_TRIG = 0x5a;	   // 写触发命令(0x5a)
	IAP_TRIG = 0xa5;	   // 写触发命令(0xa5)
	_nop_();
	dat = IAP_DATA; // 读IAP数据
	IapIdle();		// 关闭IAP功能
	return dat;
}

void IapProgram(int addr, char dat)
{
	IAP_CONTR = 0x80;	   // 使能IAP
	IAP_TPS = 12;		   // 设置等待参数12MHz
	IAP_CMD = 2;		   // 设置IAP写命令
	IAP_ADDRL = addr;	   // 设置IAP低地址
	IAP_ADDRH = addr >> 8; // 设置IAP高地址
	IAP_DATA = dat;		   // 写IAP数据
	IAP_TRIG = 0x5a;	   // 写触发命令(0x5a)
	IAP_TRIG = 0xa5;	   // 写触发命令(0xa5)
	_nop_();
	IapIdle(); // 关闭IAP功能
}

void IapErase(int addr)
{
	IAP_CONTR = 0x80;	   // 使能IAP
	IAP_TPS = 12;		   // 设置等待参数12MHz
	IAP_CMD = 3;		   // 设置IAP擦除命令
	IAP_ADDRL = addr;	   // 设置IAP低地址
	IAP_ADDRH = addr >> 8; // 设置IAP高地址
	IAP_TRIG = 0x5a;	   // 写触发命令(0x5a)
	IAP_TRIG = 0xa5;	   // 写触发命令(0xa5)
	_nop_();			   //
	IapIdle();			   // 关闭IAP功能
}