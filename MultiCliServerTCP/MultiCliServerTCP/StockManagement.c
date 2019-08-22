#include "StockManagement.h"

void FirstInit() //TotalDataArr를 포함한 초기 데이터 초기화. 매일 마감 정산후 호출
{
	memset(MonthSaleAccount, 0, sizeof(MonthSaleAccount));
	memset(DaySaleAccount, 0, sizeof(DaySaleAccount));
	memset(DailyTypeSale, 0, sizeof(DailyTypeSale));

	TotalArrIdx = 0;
	CoffeeBeanStock = 100; //10kg
	FruitStock = 1000; //100kg
	MilkStock = 1000;
}

void DailyInit() //매일 초기화돼야하는 데이터만 초기화
{
	TotalArrIdx = 0;
	memset(DailyTypeSale, 0, sizeof(DailyTypeSale));

}

int GetTotalArrSize() //TotalDataArr의 개수 반환
{
	return sizeof(TotalDataArr) / sizeof(TotalDataArr[0]);
}

void CopyRecvData(RecvData* from, RecvData* to) //from 구조체의 내용을 to 구조체에 복사
{
	to->type = from->type;
	to->price = from->price;
	to->date = from->date;
}

void AddRecvData(RecvData rd) //POS로부터 받은 데이터를 TotalDataArr에 추가
{
	if (GetTotalArrSize() == TotalArrIdx - 1)
	{
		printf("하루 가용 주문이 가득찼습니다.\n");
	}

	TotalDataArr[TotalArrIdx].type = rd.type;
	TotalDataArr[TotalArrIdx].price = rd.price;
	TotalDataArr[TotalArrIdx].date = rd.date;

	TotalArrIdx++;
}

int CalDayAccount() //매일 마감 시간에 서버에서 TotalDataArr 정산 후, 발주량 계산하여 DaySaleAccount의 해당에 저장 및 반환
{
	time_t t = time(NULL);
	tm curTime = *localtime(&t); //현재 시간 받아옴
	int dsum = 0;

	int i;
	for (i = 0; i < TotalArrIdx; i++)
	{
		dsum = TotalDataArr[i].price + dsum;
	}

	DaySaleAccount[curTime.tm_mday] = dsum;
	return dsum;
}

int CalMonthAccount() //매일 서버에서 정산 후, 오늘 까지의 이번 달 발주량 계산하여 MonthSaleAccount의 해당 월에 저장 및 반환
{
	time_t t = time(NULL);
	tm curTime = *localtime(&t); //현재 시간 받아옴
	int msum = 0;

	int i;
	for (i = 1; i <= 31; i++)
	{
		msum += DaySaleAccount[i];
	}

	MonthSaleAccount[curTime.tm_mon + 1] = msum;
	return msum;
}

void PrintAllTypeSale() //모든 타입에 대해 오늘 & 지금까지의 주문 횟수 출력
{
	printf("- 오늘 제품별 판매량 -\n");
	printf("Type A 주문: %d회\n", DailyTypeSale[0]);
	printf("Type B 주문: %d회\n", DailyTypeSale[1]);
	printf("Type C 주문: %d회\n", DailyTypeSale[2]);
}

void PrintAllMaterialUsed() //모든 재료들에 대해 오늘 사용량 출력
{
	printf("- 오늘 재료별 사용량 -\n");
	printf("커피 사용량: %dg\n", TodayUsingCoffeBean * 100);
	printf("우유 사용량: %dg\n", TodayUsingMilk * 100);
	printf("과일 사용량: %dg\n", TodayUsingFruit * 100);
}

BOOL IsDeadline(int h, int m, int s) //현재 시스템 시간이 마감 시간에 해당한다면 1, 아니라면 0 반환
{
	time_t t = time(NULL);
	tm curTime = *localtime(&t); //현재 시간 받아옴

	if (curTime.tm_hour == h && curTime.tm_min == m && curTime.tm_sec == s)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void RequestCoffeBean()
{
	printf("\n커피 원두를 발주 요청했습니다.\n");

	CoffeeBeanStock += 100;
	printf("발주 요청 후 원두 재고: %dg\n\n", CoffeeBeanStock * 100);
}

void RequestMilk()
{
	printf("\n우유 발주 요청했습니다.\n");

	MilkStock += 1000;
	printf("발주 요청 후 우유 재고: %dg\n\n", MilkStock * 100);
}

void RequestFruit()
{
	printf("\n과일 발주 요청했습니다.\n");

	FruitStock += 1000;
	printf("발주 요청 후 과일 재고: %dg\n\n", FruitStock * 100);
}

void ChkOrderItem() //원재료 발주 요청해야하는지 판별하여 요청
{
	printf("\n현재 원두 재고: %dg\n", CoffeeBeanStock * 100);
	printf("현재 우유 재고: %dg\n", MilkStock * 100);
	printf("현재 과일 재고: %dg\n", FruitStock * 100);

	if (CoffeeBeanStock < 100) //원두가 10kg 미만 남았다면 발주 필요
	{
		RequestCoffeBean();
	}

	if (MilkStock < 100) //우유가 10kg 미만 남았다면 발주 필요
	{
		RequestMilk();
	}

	if (FruitStock < 100) //과일이 10kg 미만 남았다면 발주 필요
	{
		RequestFruit();
	}
}

void SaveRecordDB() //하루 판매 & 발주 정보를 DB에 저장
{
	FILE* fp;
	fp = fopen("AccountDB.txt", "a");

	struct tm tm;
	time_t t = time(NULL);
	struct tm curTime = *localtime(&t); //현재 시간 받아옴


	fprintf(fp, "-------- %d/%d/%d 일일 정산 시작 --------\n", curTime.tm_year + 1900, curTime.tm_mon + 1, curTime.tm_mday);
	fprintf(fp, "-------- 오늘 판매액: %d    이번 달 판매액: %d --------\\n", CalDayAccount(), CalMonthAccount());


	fprintf(fp, "- 오늘 제품별 판매량 -\n");
	fprintf(fp, "Type A 주문: %d회\n", DailyTypeSale[0]);
	fprintf(fp, "Type B 주문: %d회\n", DailyTypeSale[1]);
	fprintf(fp, "Type C 주문: %d회\n", DailyTypeSale[2]);


	fprintf(fp, "- 오늘 재료별 사용량 -\n");
	fprintf(fp, "커피 사용량: %dg\n", TodayUsingCoffeBean * 100);
	fprintf(fp, "우유 사용량: %dg\n", TodayUsingMilk * 100);
	fprintf(fp, "과일 사용량: %dg\n", TodayUsingFruit * 100);


	fprintf(fp, "-------- %d/%d%d 일일 정산 끝 --------\n", curTime.tm_year + 1900, curTime.tm_mon + 1, curTime.tm_mday);


	fclose(fp);
}