#ifndef STOCK_MNG_H
#define STOCK_MNG_H

#pragma warning(disable: 4996)

#include <time.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define true 1
#define false 0

typedef int BOOL;
typedef struct tm tm;

typedef struct _RecvData
{
	char type; //�ȸ� ��ǰ ����
	int price; //�ȸ� ��ǰ ����
	tm date; //���� �ð�
}RecvData;

RecvData TotalDataArr[5000]; //���� ����~�����ð��� ������ POS��κ��� ���� �����͵��� �� ����
int TotalArrIdx; //TotalDataArr�� ����ִ� �ε��� �� ���� ���� �ε��� Idx
int TotalArrSize; //���� TotalDataArr�� ��ü ������

int MonthSaleAccount[13]; //1~12�� �Ǹž� ����
int DaySaleAccount[32]; //1~31�� �Ǹž� ����

int CoffeeBeanStock; //���� Ŀ�ǿ��� 100g����
int MilkStock; //���� ���� 100g����
int FruitStock; //���� ���� 100g����
int TodayUsingCoffeBean; //���� Ŀ�� ��뷮
int TodayUsingMilk; //���� ���� ��뷮
int TodayUsingFruit; //���� ���� ��뷮

int DailyTypeSale[3]; // 0:A ,1:B, 2:C

void FirstInit(); //�ý��� ù ������ ����Ǵ� ������ �ʱ�ȭ

void DailyInit(); //���� �ʱ�ȭ�ž��ϴ� �����͸� �ʱ�ȭ

int GetTotalArrSize(); //TotalDataArr�� ���� ��ȯ

void CopyRecvData(RecvData* from, RecvData* to); //from ����ü�� ������ to ����ü�� ����

void AddRecvData(RecvData rd); //POS�κ��� ���� �����͸� TotalDataArr�� �߰�

int CalDayAccount(); //���� ���� �ð��� �������� ���� �� ���ַ� ����Ͽ� ��ȯ

int CalMonthAccount(); //�ſ� ���� �ð��� �������� ���� �� ���ַ� ����Ͽ� ��ȯ

void PrintAllTypeSale(); //��� Ÿ�Կ� ���� ���� �ֹ� Ƚ�� ���

void PrintAllMaterialUsed(); //��� ���鿡 ���� ���� ��뷮 ���

BOOL IsDeadline(int h, int m, int s); //���� �ý��� �ð��� ���� �ð��� �ش��Ѵٸ� 1, �ƴ϶�� 0 ��ȯ

void RequestCoffeBean(); //Ŀ�ǿ��� ��û

void RequestFruit(); //���� ��û

void ChkOrderItem(); //����� ���� ��û�ؾ��ϴ��� �Ǻ��Ͽ� ��û

void SaveRecordDB(); //�Ϸ� �Ǹ� & ���� ������ DB�� ����

#endif