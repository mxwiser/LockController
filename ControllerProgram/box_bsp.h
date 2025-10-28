#ifndef BOX_BSP_H
#define BOX_BSP_H

#include <STC8H.H>
#include <intrins.h>

#define VERSION_L 0x01
#define VERSION_H 0x00

#define CHW_0 P17
#define CHW_1 P16
#define CHW_2 P47
#define CHW_3 P11
#define CHW_4 P62
#define CHW_5 P61
#define CHW_6 P06
#define CHW_7 P05
#define CHW_8 P04
#define CHW_9 P03
#define CHW_10 P00
#define CHW_11 P46
#define CHW_12 P76
#define CHW_13 P75
#define CHW_14 P26
#define CHW_15 P25
#define CHW_16 P22
#define CHW_17 P21

#define CHR_0 P15
#define CHR_1 P14
#define CHR_2 P10
#define CHR_3 P63
#define CHR_4 P60
#define CHR_5 P07
#define CHR_6 P53
#define CHR_7 P52
#define CHR_8 P02
#define CHR_9 P01
#define CHR_10 P45
#define CHR_11 P77
#define CHR_12 P74
#define CHR_13 P27
#define CHR_14 P24
#define CHR_15 P23
#define CHR_16 P20
#define CHR_17 P44
#define CH_MAX 18


#define SYS_LED P13
#define RS485_EN P54 
#define RS485_TX  P54=1
#define RS485_RX  P54=0


// #define KEY_GROUP_0 P51
// #define KEY_GROUP_1 P50
// #define KEY_GROUP_2 P34
// #define KEY_GROUP_3 P33
// #define KEY_GROUP_4 P32

#define KEY_GROUP_0 P32
#define KEY_GROUP_1 P33
#define KEY_GROUP_2 P34
#define KEY_GROUP_3 P50
#define KEY_GROUP_4 P51


#define KEY_OPEN P36
#define KEY_SET P35

// #define LED_GROUP_0 P67
// #define LED_GROUP_1 P66
// #define LED_GROUP_2 P65
// #define LED_GROUP_3 P64
// #define LED_GROUP_4 P40

#define LED_GROUP_0 P40
#define LED_GROUP_1 P64
#define LED_GROUP_2 P65
#define LED_GROUP_3 P66
#define LED_GROUP_4 P67







void UartSend(char dat);
void delay_1s();
void BOX_Init();
void delay(int ms);
void Drain_set(unsigned char gpio_pin,unsigned char set);
unsigned char  feedback_read(char gpio_pin);
void loop();
void cmd_process();
void open_lock_cmd(unsigned char i);
void open_all_lock_cmd(unsigned char i);
void get_lock_state_cmd(unsigned char i);
void get_state_buffer();
void output_enable(unsigned char i);
void output_disable(unsigned char i);
void get_chipid(unsigned char i);
void IapIdle();
void IapProgram(int addr, char dat);
void IapErase(int addr);
char IapRead(int addr);
void open_all_lock();
void open_all_lock_key();
void get_version(unsigned char i);
void set_vector(unsigned char i);
void get_vector(unsigned char i);
#endif