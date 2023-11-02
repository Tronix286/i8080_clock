/*
    ���������� ��� ����������� �������� ������ �� �������� ��� Arduino
    ������������:
    GitHub: https://github.com/GyverLibs/Forecaster
    �����������:
    - ����������� �������������� �������� ������ �� ��������� ���������
    - ��������� ��������, �����������, ������ ��� ��. ���� � ����� ����
    - ����������� ������ �������� ��� ������ ������������
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
    
    �������� ��
    https://integritext.net/DrKFS/zambretti.htm

    ������:
    v1.0 - �����
    v1.1 - ������� ����� ������ �������� �� 3 ����
    v1.2 - ������������� esp8266/32
*/

#include <stdlib.h>
#include "math.h"

#define _FC_SIZE 6  // ������ ������. ���������� �� 3 ����, ��� ������� 6 - ������ 30 �����

long Parr[_FC_SIZE];
float H = 0;
uint8_t start = 0;
float cast = 0;
int delta = 0;
uint8_t season = 0;

// ���������� ������ ��� ������� ���� (� ������)
void setH(int h) {
    H = h * 0.0065f;
}

// �������� ������� �������� � �� � ����������� � � (������ 30 �����)
// ����� �� ���������� ������ ��������
void addP(long P, float t) {
    P = (float)P * pow(1 - H / (t + H + 273.15), -5.257);   // ��� ������� ����
    if (!start) {
        start = 1;
        for (uint8_t i = 0; i < _FC_SIZE; i++) Parr[i] = P;
    } else {
        for (uint8_t i = 0; i < (_FC_SIZE-1); i++) Parr[i] = Parr[i + 1];
        Parr[_FC_SIZE - 1] = P;
    }
    
    // ������ ��������� �� ���������� ���������
    long sumX = 0, sumY = 0, sumX2 = 0, sumXY = 0;        
    for (int i = 0; i < _FC_SIZE; i++) {
        sumX += i;
        sumY += Parr[i];
        sumX2 += i * i;
        sumXY += Parr[i] * i;
    }
    float a = _FC_SIZE * sumXY - sumX * sumY;
    a /= _FC_SIZE * sumX2 - sumX * sumX;
    delta = a * (_FC_SIZE - 1);
    
    // ������ �������� �� Zambretti
    P /= 100;   // -> ���
    if (delta > 150) cast = 160 - 0.155 * P - season;        // rising
    else if (delta < -150) cast = 130 - 0.124 * P + season;  // falling
    else cast = 138 - 0.133 * P;                             // steady
    if (cast < 0) cast = 0;
}

#if 0
// �������� ������� �������� � ��.��.�� � ����������� � � (������ 30 �����)
void addPmm(float P, float t) {
    addP(P * 133.322f, t);
}
#endif

// ���������� ����� (1-12)
// 0 ����� ��������� ����������
void setMonth(uint8_t month) {
    if (month == 0) season = 0;
    else season = (month >= 4 && month <= 9) ? 2 : 1;
    /*
    if (month == 12) month = 0;
    month /= 3;                         // 0 ����, 1 �����, 2 ����, 3 �����
    season = month * 0.5 + 1;           // 1, 1.5, 2, 2.5
    if (season == 2.5) season = 1.5;    // 1, 1.5, 2, 1.5
    */
}

// �������� ������� (0 ������� ������... 10 ������-�����)
float getCast() {
    return cast;
}

#if 0
// �������� ��������� �������� � �� �� 3 ����
int getTrend() {
    return delta;
}
#endif

