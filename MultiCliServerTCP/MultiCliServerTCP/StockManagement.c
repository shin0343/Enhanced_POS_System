#include "StockManagement.h"

 RecvData TotalDataArr[5000];
 int TotalArrIdx;
 int TotalArrSize;
 int CoffeeBeanStock;
 int FruitStock;


void FirstInit() //TotalDataArr�� ������ �ʱ� ������ �ʱ�ȭ. ���� ���� ������ ȣ��
{
	memset(MonthSaleAccount, 0, sizeof(MonthSaleAccount));
	memset(DaySaleAccount, 0, sizeof(DaySaleAccount));
	TotalArrIdx = 0;
	CoffeeBeanStock = 100; //10kg
	FruitStock = 1000; //100kg
	MilkStock = 1000;
}

void DailyInit() //���� �ʱ�ȭ�ž��ϴ� �����͸� �ʱ�ȭ
{
	TotalArrIdx = 0;
}

int GetTotalArrSize() //TotalDataArr�� ���� ��ȯ
{
	return sizeof(TotalDataArr) / sizeof(TotalDataArr[0]);
}

void CopyRecvData(RecvData* from, RecvData* to) //from ����ü�� ������ to ����ü�� ����
{
	to->type = from->type;
	to->price = from->price;
	to->date = from->date;
}

void AddRecvData(RecvData rd) //POS�κ��� ���� �����͸� TotalDataArr�� �߰�
{
	if (GetTotalArrSize() == TotalArrIdx - 1)
	{
		printf("�Ϸ� ���� �ֹ��� ����á���ϴ�.\n");
	}

	TotalDataArr[TotalArrIdx].type = rd.type;
	TotalDataArr[TotalArrIdx].price = rd.price;
	TotalDataArr[TotalArrIdx].date = rd.date;

	TotalArrIdx++;
}

int CalDayAccount() //���� ���� �ð��� �������� TotalDataArr ���� ��, ���ַ� ����Ͽ� DaySaleAccount�� �ش翡 ���� �� ��ȯ
{
	time_t t = time(NULL);
	tm curTime = *localtime(&t); //���� �ð� �޾ƿ�
	int dsum = 0;

	int i;
	for (i = 0; i < TotalArrIdx; i++)
	{
		dsum = TotalDataArr[i].price + dsum;
	}

	DaySaleAccount[curTime.tm_mday] = dsum;
	return dsum;
}

int CalMonthAccount() //���� �������� ���� ��, ���� ������ �̹� �� ���ַ� ����Ͽ� MonthSaleAccount�� �ش� ���� ���� �� ��ȯ
{
	time_t t = time(NULL);
	tm curTime = *localtime(&t); //���� �ð� �޾ƿ�
	int msum = 0;

	int i;
	for (i = 1; i <= 31; i++)
	{
		msum += DaySaleAccount[i];
	}

	MonthSaleAccount[curTime.tm_mon + 1] = msum;
	return msum;
}

BOOL IsDeadline(int h, int m, int s) //���� �ý��� �ð��� ���� �ð��� �ش��Ѵٸ� 1, �ƴ϶�� 0 ��ȯ
{
	time_t t = time(NULL);
	tm curTime = *localtime(&t); //���� �ð� �޾ƿ�

	if (curTime.tm_hour == h && curTime.tm_min == m && curTime.tm_sec==s)
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
	printf("\nĿ�� ���θ� ���� ��û�߽��ϴ�.\n");

	CoffeeBeanStock += 100;
	printf("���� ��û �� ���� ���: %dg\n\n", CoffeeBeanStock * 100);
}

void RequestMilk()
{
	printf("\n���� ���� ��û�߽��ϴ�.\n");

	MilkStock += 1000;
	printf("���� ��û �� ���� ���: %dg\n\n", MilkStock * 100);
}

void RequestFruit()
{
	printf("\n���� ���� ��û�߽��ϴ�.\n");

	FruitStock += 1000;
	printf("���� ��û �� ���� ���: %dg\n\n", FruitStock * 100);
}

void ChkOrderItem() //����� ���� ��û�ؾ��ϴ��� �Ǻ��Ͽ� ��û
{
	printf("\n���� ���� ���: %dg\n", CoffeeBeanStock * 100);
	printf("���� ���� ���: %dg\n", MilkStock * 100);
	printf("���� ���� ���: %dg\n", FruitStock * 100);

	if (CoffeeBeanStock < 100) //���ΰ� 10kg �̸� ���Ҵٸ� ���� �ʿ�
	{
		RequestCoffeBean();
	}

	if (MilkStock < 100) //������ 10kg �̸� ���Ҵٸ� ���� �ʿ�
	{
		RequestMilk();
	}

	if (FruitStock < 100) //������ 10kg �̸� ���Ҵٸ� ���� �ʿ�
	{
		RequestFruit();
	}
}