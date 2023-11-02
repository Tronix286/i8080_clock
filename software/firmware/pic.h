//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//����Ӳ������Ƭ��STC12C5A60S2,����30M  ��Ƭ��������ѹ3.3V
//QDtech-LCDҺ������ for C51
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/7/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
//********************************************************************************
//=========================================��Դ����================================================//
//5V��DC 5V��Դ
//GND�ӵ�
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ16λ����
//Һ����ģ��             C51��Ƭ��
// DB0~DB7       ��       P00~P07        //���ݵ�8λ
// DB8~DB15      ��       P20~P27        //���ݸ�8λ
//=======================================Һ���������߽���==========================================//
//Һ����ģ��             C51��Ƭ��
// LCD_CS        ��        P13           //Ƭѡ�����ź�
// LCD_RST       ��        P33           //��λ�ź�
// LCD_RS        ��        P12           //����/����ѡ������ź�
// LCD_WR        ��        P11           //д�����ź�
// LCD_RD        ��        P10           //�������ź�
//=========================================����������=========================================//
//STC89C52RC��Ƭ��ROM̫С���޷����д��������ܵĳ������Դ���������Ҫ����
//��ʹ�ô�������ģ�鱾��������������ɲ�����
//������ʹ�õ�������������ΪSPI
//������ģ��             C51��Ƭ��
//  T_CLK        ��        P36          //������SPIʱ���ź�
//  T_CS         ��        P37          //������Ƭѡ�����ź�
//  T_DIN        ��        P34          //������SPIд�ź�
//  T_DO         ��        P35          //������SPI���ź�
//  T_IRQ        ��        P40          //��������Ӧ����źţ��絥Ƭ����P4�飬�����и�����������IO���޸Ĵ��붨��
//**************************************************************************************************/	
#ifndef __PIC_H
#define __PIC_H 

unsigned char gImage_temperature[512] = { /* 0X10,0X04,0X00,0X20,0X00,0X20, */
0X00,0X00,0X00,0X00,0X06,0XDF,0XB2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X6F,0XA7,0XDE,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XDA,0X00,0X0F,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XF5,0X00,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XF5,0X00,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XAA,0XAA,0X30,0XF5,0X00,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XAA,0XAA,0X30,0XF5,0X00,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XF5,0X00,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XBF,0X60,0XF5,0X00,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X25,0X00,0XF5,0X00,0X0A,0XA0,0X00,0X06,0X9A,0X83,0X00,0X02,0X85,0X00,
0X00,0X00,0X25,0X00,0XF5,0X00,0X0A,0XA0,0X00,0XCF,0XBA,0XDF,0X60,0X0F,0XEF,0X50,
0X00,0X00,0XBF,0X60,0XF5,0X00,0X0A,0XA0,0X06,0XF3,0X00,0X08,0XF1,0X3F,0X5E,0X80,
0X00,0X00,0X00,0X00,0XF5,0X00,0X0A,0XA0,0X09,0XB0,0X00,0X01,0XF3,0X0C,0XFF,0X20,
0X03,0XAA,0XAA,0X30,0XF5,0X06,0X0A,0XA0,0X0A,0XA0,0X00,0X00,0X10,0X00,0X30,0X00,
0X03,0XAA,0XAA,0X30,0XF5,0X5F,0X0A,0XA0,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XF5,0X5F,0X0A,0XA0,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XBF,0X60,0XF5,0X5F,0X0A,0XA0,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X25,0X00,0XF5,0X5F,0X0A,0XA0,0X0A,0XA0,0X00,0X00,0X60,0X00,0X00,0X00,
0X00,0X00,0X25,0X00,0XF5,0X5F,0X0A,0XA0,0X08,0XD0,0X00,0X03,0XF3,0X00,0X00,0X00,
0X00,0X00,0XBF,0X60,0XF5,0X5F,0X0A,0XA0,0X03,0XF8,0X10,0X3C,0XD0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XF5,0X5F,0X0A,0XA0,0X00,0X6F,0XFF,0XFD,0X30,0X00,0X00,0X00,
0X03,0XAA,0XAA,0X30,0XF5,0X5F,0X0A,0XA0,0X00,0X01,0X45,0X30,0X00,0X00,0X00,0X00,
0X03,0XAA,0XAA,0X30,0XF5,0X5F,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XF5,0X5F,0X0A,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X04,0XF4,0X5F,0X09,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0E,0XA0,0X8F,0X21,0XE8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X2F,0X25,0XFE,0XF1,0X8D,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X4F,0X08,0XE5,0XF3,0X6F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X1F,0X42,0XFF,0XC0,0X9C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0B,0XD0,0X03,0X03,0XF6,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X01,0XEE,0X86,0X9F,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X08,0XDF,0XC6,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

unsigned char gImage_humidity[512] = { /* 0X10,0X04,0X00,0X20,0X00,0X20, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X8E,0XDA,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X05,0XF4,0X2F,0X60,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X2F,0X70,0X06,0XF3,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XDB,0X00,0X00,0X9E,0X10,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X0A,0XD0,0X00,0X00,0X0C,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X7F,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X04,0XF5,0X00,0X00,0X00,0X00,0X0E,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X1E,0X80,0X00,0X00,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XCB,0X00,0X00,0X00,0X00,0X00,0X00,0X6C,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X09,0XE1,0X00,0X00,0X00,0X00,0X00,0X00,0X1D,0X90,0X00,0X00,0X00,
0X00,0X00,0X00,0X6F,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XF6,0X00,0X00,0X00,
0X00,0X00,0X02,0XF6,0X00,0X19,0XA5,0X00,0X00,0X10,0X00,0X00,0X6F,0X20,0X00,0X00,
0X00,0X00,0X0B,0XB0,0X00,0XDC,0X9F,0X40,0X00,0XD7,0X00,0X00,0X0B,0XB0,0X00,0X00,
0X00,0X00,0X3F,0X20,0X02,0XF1,0X0B,0X90,0X07,0XE1,0X00,0X00,0X02,0XF3,0X00,0X00,
0X00,0X00,0XAA,0X00,0X00,0XF8,0X4E,0X60,0X1E,0X60,0X00,0X00,0X00,0XBA,0X00,0X00,
0X00,0X00,0XE5,0X00,0X00,0X4E,0XF9,0X00,0X9D,0X00,0X00,0X00,0X00,0X5E,0X00,0X00,
0X00,0X02,0XF1,0X00,0X00,0X00,0X00,0X02,0XF4,0X00,0X00,0X00,0X00,0X1F,0X20,0X00,
0X00,0X04,0XF0,0X00,0X00,0X00,0X00,0X0B,0XB0,0X00,0X00,0X00,0X00,0X0F,0X40,0X00,
0X00,0X05,0XE0,0X00,0X00,0X00,0X00,0X5F,0X20,0X01,0X00,0X00,0X00,0X0E,0X50,0X00,
0X00,0X04,0XF0,0X00,0X00,0X00,0X00,0XD8,0X00,0XCF,0XF6,0X00,0X00,0X0F,0X40,0X00,
0X00,0X02,0XF1,0X00,0X00,0X00,0X07,0XE0,0X07,0XD2,0X6F,0X00,0X00,0X1F,0X20,0X00,
0X00,0X00,0XF4,0X00,0X00,0X00,0X1F,0X60,0X08,0XB0,0X2F,0X10,0X00,0X4F,0X00,0X00,
0X00,0X00,0XB9,0X00,0X00,0X00,0X9C,0X00,0X03,0XFB,0XDC,0X00,0X00,0X9A,0X00,0X00,
0X00,0X00,0X5F,0X10,0X00,0X00,0X63,0X00,0X00,0X38,0X70,0X00,0X01,0XF5,0X00,0X00,
0X00,0X00,0X0C,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X09,0XC0,0X00,0X00,
0X00,0X00,0X04,0XF5,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X5F,0X30,0X00,0X00,
0X00,0X00,0X00,0X7F,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XF7,0X00,0X00,0X00,
0X00,0X00,0X00,0X08,0XF6,0X00,0X00,0X00,0X00,0X00,0X00,0X6F,0X80,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X6F,0XC3,0X00,0X00,0X00,0X00,0X3C,0XF6,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0XAF,0XD9,0X64,0X46,0X9D,0XFA,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0X6B,0XDF,0XFD,0XB6,0X10,0X00,0X00,0X00,0X00,0X00,
};

unsigned char gImage_presure[512] = { /* 0X10,0X04,0X00,0X20,0X00,0X20, */
0X00,0X00,0X00,0X01,0XAE,0XFF,0XC5,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0D,0X83,0X12,0X5E,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X4D,0X00,0X00,0X05,0XB0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0X00,0X04,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0X00,0X04,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XB3,0X04,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XA2,0X04,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0X00,0X04,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0X51,0X04,0XC0,0X00,0X77,0X73,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XC4,0X04,0XC0,0X03,0XF9,0XAE,0X70,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XC4,0X04,0XC0,0X03,0XD0,0X02,0XF0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XC4,0X04,0XC0,0X03,0XD0,0X00,0XF0,0X18,0X71,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XC4,0X04,0XC0,0X03,0XD0,0X2A,0XB0,0X18,0XBB,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XC4,0X04,0XC0,0X03,0XD0,0XC9,0X10,0X26,0X7E,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XC4,0X04,0XC0,0X03,0XD0,0X00,0X01,0XEB,0XBE,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0X82,0X04,0XC0,0X03,0XD0,0X00,0X04,0XD0,0X2E,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0X00,0X04,0XC0,0X02,0XC0,0X00,0X00,0XBF,0XFE,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0X61,0X04,0XC0,0X00,0X00,0X00,0X00,0X00,0X10,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XC4,0X04,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X5B,0X00,0XC4,0X04,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XC9,0X00,0XC4,0X02,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0A,0XA0,0X00,0XC7,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X4E,0X00,0X00,0X7D,0XE3,0X00,0X50,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XA7,0X06,0X50,0X00,0X6E,0X00,0XF1,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XD3,0X0D,0X30,0X00,0X0C,0X40,0XC4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XE2,0X0F,0X10,0X00,0X0A,0X60,0XB5,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0XC4,0X0C,0X60,0X00,0X0E,0X30,0XD4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X89,0X03,0XE5,0X02,0XBA,0X02,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X1F,0X20,0X3D,0XFF,0X80,0X0B,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X06,0XE2,0X00,0X00,0X00,0X9D,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X7F,0X93,0X12,0X6D,0XC1,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X02,0X9D,0XFE,0XB5,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

unsigned char gImage_sun[512] = { /* 0X10,0X04,0X00,0X20,0X00,0X20, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X06,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X0B,0X30,0X00,0X00,0X00,0X06,0X60,0X00,0X00,0X00,0X03,0XB0,0X00,0X00,
0X00,0X00,0X03,0XE3,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3E,0X30,0X00,0X00,
0X00,0X00,0X00,0X3E,0X30,0X00,0X37,0XBE,0XEB,0X73,0X00,0X03,0XE3,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XB0,0X09,0XD8,0X41,0X14,0X7D,0X90,0X0C,0X30,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XE8,0X00,0X00,0X00,0X00,0X8E,0X30,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X0E,0X40,0X00,0X00,0X00,0X00,0X04,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X98,0X00,0X00,0X00,0X00,0X00,0X00,0X89,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XD0,0X00,0X00,0X00,0X00,0X00,0X00,0X0D,0X30,0X00,0X00,0X00,
0X00,0X00,0X00,0X07,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X70,0X00,0X00,0X00,
0X00,0X00,0X00,0X0B,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0XB0,0X00,0X00,0X00,
0X06,0X88,0X86,0X0E,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,0X68,0X88,0X60,
0X06,0X88,0X86,0X0E,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,0X68,0X88,0X60,
0X00,0X00,0X00,0X0B,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0XB0,0X00,0X00,0X00,
0X00,0X00,0X00,0X07,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X70,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XD0,0X00,0X00,0X00,0X00,0X00,0X00,0X0D,0X30,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X98,0X00,0X00,0X00,0X00,0X00,0X00,0X89,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X0E,0X40,0X00,0X00,0X00,0X00,0X04,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XE8,0X00,0X00,0X00,0X00,0X8E,0X30,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0XB0,0X09,0XD8,0X41,0X14,0X7D,0X90,0X0B,0X30,0X00,0X00,0X00,
0X00,0X00,0X00,0X3E,0X30,0X00,0X37,0XBE,0XEB,0X73,0X00,0X03,0XE3,0X00,0X00,0X00,
0X00,0X00,0X03,0XE3,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3E,0X30,0X00,0X00,
0X00,0X00,0X0C,0X30,0X00,0X00,0X00,0X06,0X60,0X00,0X00,0X00,0X03,0XB0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X06,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

unsigned char gImage_cloudy[512] = { /* 0X10,0X04,0X00,0X20,0X00,0X20, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X56,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X78,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X30,0X00,0X00,0X00,0X56,0X00,0X00,0X00,0X02,0X20,0X00,
0X00,0X00,0X00,0X00,0X02,0XE4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3E,0X30,0X00,
0X00,0X00,0X00,0X00,0X00,0X3E,0X30,0X02,0X7C,0XEE,0XD8,0X20,0X02,0XE4,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X02,0X20,0X7F,0X84,0X11,0X38,0XE8,0X02,0X30,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0XB1,0X00,0X00,0X00,0X0B,0XB0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X7C,0X00,0X00,0X00,0X00,0X00,0XA8,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X02,0XE1,0X00,0X00,0X00,0X00,0X00,0X1E,0X20,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X07,0X90,0X00,0X00,0X00,0X00,0X00,0X07,0X80,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X0B,0X40,0X00,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0XBF,0XFD,0X70,0X00,0X00,0X00,0X00,0X00,0XF0,0X58,0X60,
0X00,0X00,0X00,0X02,0XDB,0X61,0X13,0XAE,0X30,0X00,0X00,0X00,0X00,0XE0,0X58,0X60,
0X00,0X00,0X00,0X1E,0X70,0X00,0X00,0X05,0XE2,0X00,0X00,0X00,0X02,0XD0,0X00,0X00,
0X00,0X00,0X00,0XA7,0X00,0X00,0X00,0X00,0X7C,0X46,0X61,0X00,0X08,0X80,0X00,0X00,
0X00,0X00,0X01,0XF1,0X00,0X00,0X00,0X00,0X0C,0XB8,0XAE,0X70,0X1E,0X20,0X00,0X00,
0X00,0X00,0X04,0XB0,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XC8,0XB8,0X00,0X00,0X00,
0X00,0X00,0X06,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XB0,0X00,0X00,0X00,
0X00,0X00,0X05,0XB0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X09,0XFE,0XB6,0X00,0X00,
0X00,0X01,0X8D,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X4C,0XC1,0X00,
0X00,0X3E,0X83,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X8C,0X00,
0X00,0XE3,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0X60,
0X08,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0XB0,
0X0C,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,
0X0E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,
0X0C,0X30,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0XB0,
0X08,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0X60,
0X01,0XD3,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X8C,0X00,
0X00,0X3E,0X83,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X4C,0XC1,0X00,
0X00,0X00,0X8C,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XB6,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

unsigned char gImage_cld_rain[512] = { /* 0X10,0X04,0X00,0X20,0X00,0X20, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X06,0X60,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X05,0X50,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X30,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X8D,0XFF,0XD8,0X10,0X3E,0X20,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X6E,0X95,0X10,0X38,0XF5,0X22,0X00,0X00,
0X00,0X00,0X00,0X00,0X2A,0XDF,0XEB,0X45,0XF4,0X00,0X00,0X00,0X3D,0X50,0X00,0X00,
0X00,0X00,0X00,0X07,0XE6,0X20,0X15,0XDF,0X40,0X00,0X00,0X00,0X03,0XE1,0X00,0X00,
0X00,0X00,0X00,0X4D,0X10,0X00,0X00,0X09,0XB0,0X00,0X00,0X00,0X00,0X98,0X00,0X00,
0X00,0X00,0X00,0XE3,0X00,0X00,0X00,0X00,0XCA,0XB8,0X30,0X00,0X00,0X4C,0X00,0X00,
0X00,0X00,0X04,0XB0,0X00,0X00,0X00,0X00,0X38,0X57,0XCE,0X10,0X00,0X1E,0X06,0X60,
0X00,0X00,0X08,0X70,0X00,0X00,0X00,0X00,0X00,0X00,0X06,0XB0,0X00,0X1E,0X04,0X40,
0X00,0X00,0X09,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC9,0X74,0X5A,0X00,0X00,
0X00,0X00,0X1A,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X29,0X9B,0XF9,0X00,0X00,
0X00,0X0A,0XEA,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X2E,0X50,0X00,
0X00,0XCA,0X00,0X00,0X00,0X00,0X00,0X99,0X00,0X00,0X00,0X00,0X00,0X02,0XD3,0X00,
0X07,0XB0,0X00,0X00,0X00,0X00,0X01,0XEE,0X10,0X00,0X00,0X00,0X00,0X00,0X78,0X00,
0X0C,0X50,0X00,0X00,0X00,0X00,0X09,0X99,0X90,0X00,0X00,0X00,0X00,0X00,0X3C,0X00,
0X0F,0X10,0X00,0X00,0X00,0X00,0X0E,0X33,0XE0,0X00,0X00,0X00,0X00,0X00,0X1E,0X00,
0X0D,0X30,0X00,0X00,0X00,0X00,0X3D,0X00,0XD3,0X00,0X00,0X00,0X00,0X00,0X4B,0X00,
0X09,0X70,0X00,0X00,0X00,0X00,0X69,0X00,0X96,0X09,0X90,0X00,0X00,0X00,0XA7,0X00,
0X01,0XE4,0X00,0X00,0X01,0XE1,0X1E,0X44,0XE1,0X1E,0XE1,0X00,0X00,0X07,0XC0,0X00,
0X00,0X4E,0X84,0X00,0X09,0XE9,0X04,0XBB,0X40,0X99,0X99,0X00,0X14,0XBC,0X20,0X00,
0X00,0X01,0X8C,0X80,0X1F,0X3F,0X10,0X00,0X00,0XE3,0X3E,0X01,0XDA,0X60,0X00,0X00,
0X00,0X00,0X00,0X00,0X6B,0X0B,0X60,0X00,0X03,0XD0,0X0D,0X30,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XB5,0X05,0XB0,0X00,0X06,0X90,0X09,0X60,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XE1,0X01,0XE0,0X00,0X01,0XE4,0X4E,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X99,0X29,0X90,0X00,0X00,0X4B,0XB4,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X18,0XD8,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

unsigned char gImage_rainy[512] = { /* 0X10,0X04,0X00,0X20,0X00,0X20, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X29,0XFE,0XEE,0XA2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X05,0XE8,0X10,0X01,0X7E,0X50,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X3E,0X20,0X00,0X00,0X03,0XE2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XC5,0X00,0X00,0X00,0X00,0X6D,0X8A,0X83,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X03,0XD0,0X00,0X00,0X00,0X00,0X09,0X85,0XAF,0XA0,0X00,0X00,0X00,0X00,
0X00,0X00,0X06,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC9,0X00,0X00,0X00,0X00,
0X00,0X00,0X07,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X41,0X00,0X00,0X00,
0X00,0X00,0X06,0XB0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XE9,0X00,0X00,
0X00,0X03,0XBF,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1A,0XE3,0X00,
0X00,0X6E,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X6D,0X00,
0X02,0XE1,0X00,0X00,0X00,0X00,0X00,0X2F,0X20,0X00,0X00,0X00,0X00,0X00,0X0B,0X70,
0X0A,0X60,0X00,0X00,0X00,0X00,0X00,0XBD,0XA0,0X00,0X00,0X00,0X00,0X00,0X05,0XB0,
0X0D,0X10,0X00,0X00,0X00,0X00,0X02,0XF2,0XF1,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,
0X0E,0X00,0X00,0X00,0X00,0X00,0X07,0XA0,0XA6,0X00,0X00,0X00,0X00,0X00,0X03,0XD0,
0X0C,0X30,0X00,0X00,0X00,0X00,0X0C,0X40,0X4B,0X00,0X00,0X00,0X00,0X00,0X06,0XA0,
0X07,0X90,0X00,0X00,0X00,0X00,0X0E,0X10,0X1E,0X02,0XF2,0X00,0X00,0X00,0X1D,0X40,
0X00,0XD6,0X00,0X00,0X02,0XF2,0X08,0XB4,0XB7,0X0B,0XDA,0X00,0X00,0X00,0XC9,0X00,
0X00,0X2C,0XB6,0X10,0X0B,0XDA,0X00,0X6B,0X60,0X2F,0X2F,0X10,0X15,0X9E,0X90,0X00,
0X00,0X00,0X69,0X90,0X2F,0X2F,0X10,0X00,0X00,0X7A,0X0A,0X60,0XAB,0X83,0X00,0X00,
0X00,0X00,0X00,0X00,0X7A,0X0A,0X60,0X00,0X00,0XC4,0X04,0XB0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XC4,0X04,0XB0,0X00,0X00,0XE1,0X01,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XE1,0X01,0XE0,0X00,0X00,0X8B,0X4B,0X70,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X8B,0X4B,0X70,0X00,0X00,0X06,0XB6,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0XB6,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

unsigned char gImage_storm[512] = { /* 0X10,0X04,0X00,0X20,0X00,0X20, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X29,0XFE,0XEE,0XA2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X05,0XE8,0X10,0X01,0X7E,0X50,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X3E,0X20,0X00,0X00,0X03,0XE2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XC5,0X00,0X00,0X00,0X00,0X6D,0X8A,0X83,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X03,0XD0,0X00,0X00,0X00,0X00,0X09,0X85,0XAF,0XA0,0X00,0X00,0X00,0X00,
0X00,0X00,0X06,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC9,0X00,0X00,0X00,0X00,
0X00,0X00,0X07,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X41,0X00,0X00,0X00,
0X00,0X00,0X06,0XB0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XE9,0X00,0X00,
0X00,0X03,0XBF,0X90,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1A,0XE3,0X00,
0X00,0X6E,0X40,0X00,0X00,0X02,0X22,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X6D,0X00,
0X02,0XE1,0X00,0X00,0X00,0X9D,0XDD,0XF6,0X00,0X00,0X00,0X00,0X00,0X00,0X0B,0X70,
0X0A,0X60,0X00,0X00,0X00,0XE1,0X00,0XD3,0X00,0X00,0X00,0X00,0X00,0X00,0X05,0XB0,
0X0D,0X10,0X00,0X00,0X04,0XC0,0X01,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0XE0,
0X0E,0X00,0X00,0X00,0X09,0X70,0X05,0XC2,0X22,0X00,0X00,0X00,0X00,0X00,0X03,0XD0,
0X0C,0X30,0X00,0X00,0X0E,0X10,0X05,0XDD,0XDF,0X50,0X00,0X00,0X00,0X00,0X06,0XA0,
0X07,0X90,0X00,0X00,0X4D,0X22,0X22,0X00,0X6C,0X00,0X00,0X00,0X00,0X00,0X1D,0X40,
0X00,0XD6,0X00,0X00,0X5D,0XDD,0XDE,0X01,0XE2,0X0A,0XA0,0X00,0X00,0X00,0XC9,0X00,
0X00,0X2C,0XB6,0X10,0X00,0X00,0X4B,0X0B,0X70,0X3E,0XE2,0X00,0X15,0X9E,0X90,0X00,
0X00,0X00,0X69,0X90,0X00,0X00,0X78,0X5C,0X00,0XA9,0X99,0X00,0XAB,0X83,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XA7,0XE3,0X00,0XE2,0X2E,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XDD,0X80,0X03,0XC0,0X0C,0X30,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFD,0X00,0X06,0X90,0X09,0X60,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X04,0XF3,0X00,0X00,0XE6,0X6E,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X03,0X70,0X00,0X00,0X29,0X92,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

#endif