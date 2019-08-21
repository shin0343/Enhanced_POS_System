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
	char type; //팔린 제품 종류
	int price; //팔린 제품 가격
	tm date; //받은 시간
}RecvData;

RecvData TotalDataArr[5000]; //매일 개장~마감시간에 서버가 POS들로부터 받은 데이터들의 총 집합
int TotalArrIdx; //TotalDataArr의 비어있는 인덱스 중 가장 작은 인덱스 Idx
int TotalArrSize; //현재 TotalDataArr의 전체 사이즈

int MonthSaleAccount[13]; //1~12월 판매액 보관
int DaySaleAccount[32]; //1~31일 판매액 보관

int CoffeeBeanStock; //남은 커피원두 100g단위
int MilkStock; //남은 과일 100g단위
int FruitStock; //남은 과일 100g단위
int TodayUsingCoffeBean; //오늘 커피 사용량
int TodayUsingMilk; //오늘 우유 사용량
int TodayUsingFruit; //오늘 과일 사용량

int DailyTypeSale[3]; // 0:A ,1:B, 2:C

void FirstInit(); //시스템 첫 가동시 수행되는 데이터 초기화

void DailyInit(); //매일 초기화돼야하는 데이터만 초기화

int GetTotalArrSize(); //TotalDataArr의 개수 반환

void CopyRecvData(RecvData* from, RecvData* to); //from 구조체의 내용을 to 구조체에 복사

void AddRecvData(RecvData rd); //POS로부터 받은 데이터를 TotalDataArr에 추가

int CalDayAccount(); //매일 마감 시간에 서버에서 정산 후 발주량 계산하여 반환

int CalMonthAccount(); //매월 마감 시간에 서버에서 정산 후 발주량 계산하여 반환

void PrintAllTypeSale(); //모든 타입에 대해 오늘 주문 횟수 출력

void PrintAllMaterialUsed(); //모든 재료들에 대해 오늘 사용량 출력

BOOL IsDeadline(int h, int m, int s); //현재 시스템 시간이 마감 시간에 해당한다면 1, 아니라면 0 반환

void RequestCoffeBean(); //커피원두 요청

void RequestFruit(); //과일 요청

void ChkOrderItem(); //원재료 발주 요청해야하는지 판별하여 요청

void SaveRecordDB(); //하루 판매 & 발주 정보를 DB에 저장

#endif