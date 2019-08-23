#pragma comment(lib,"ws2_32")

#include <stdio.h> 
#include <winsock2.h> 
#include "StockManagement.h"

#define MAX_LINE 1024 
#define BACKLOG 5
#define PORT 25000
#define IP "192.168.0.4"


extern RecvData TotalDataArr[5000];
extern int TotalArrIdx;
extern int TotalArrSize;
extern int CoffeeBeanStock;
extern int FruitStock;
extern int TodayUsingCoffeBean; //���� Ŀ�� ��뷮
extern int TodayUsingMilk; //���� ���� ��뷮
extern int TodayUsingFruit; //���� ���� ��뷮
extern int DailyTypeSale[3]; // 0:A ,1:B, 2:C

//	�����Լ� ������� �� ����
void Error_Exit(char* str)
{
	printf("%s Error!! Exit...\n", str);
	_getch();
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET listen_fd, accept_fd, max_fd = 0, sock_fd;
	struct sockaddr_in listen_addr, accept_addr;
	char buf[MAX_LINE];
	char off[4] = "OFF";

	int readn, addr_len;
	unsigned int i, fd_num = 0;
	unsigned long nonBlk = 1;

	fd_set old_fds, new_fds;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 1;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == INVALID_SOCKET)
		return 1;

	memset((void *)&listen_addr, 0x00, sizeof(listen_addr));

	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(PORT);
	listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) == SOCKET_ERROR)
		return 1;

	if (listen(listen_fd, BACKLOG) == SOCKET_ERROR)
		return 1;

	FD_ZERO(&new_fds);
	FD_SET(listen_fd, &new_fds);

	// ��� ���� ��� �ʱ� ����
	FirstInit();

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 1000;

	// 1���� = 1�ֹ�
	while (1)
	{
		//���� �ð� üũ
		if (IsDeadline(14, 6, 0)) //(��,��,��)
		{
			printf("-------- ���� �ð��Դϴ�. --------\n\n���� �Ǹž�: %d\n", CalDayAccount());
			printf("���ñ��� �̹� �� �Ǹž�: %d\n", CalMonthAccount());
			PrintAllTypeSale();
			PrintAllMaterialUsed();

			printf("\n-------- �Ǹ� ������ ������ ...  ");
			SaveRecordDB();
			Sleep(1000);
			printf("�Ϸ� --------\n");

			DailyInit();

			for (i = 1; i <= new_fds.fd_count; i++)
			{
				sock_fd = new_fds.fd_array[i];
				send(sock_fd, off, sizeof(off), 0);
			}

			continue;
		}


		int testi = 0;
		old_fds = new_fds;

		fd_num = select(0, &old_fds, NULL, NULL, &tv);
		if (FD_ISSET(listen_fd, &old_fds))
		{
			addr_len = sizeof(struct sockaddr_in);
			accept_fd = accept(listen_fd, (struct sockaddr *)&accept_addr, &addr_len);
			if (accept_fd == INVALID_SOCKET)
			{
				continue;
			}
			FD_SET(accept_fd, &new_fds);
			printf("POS %d ���� �Ϸ�!\n", accept_fd);
		}

		for (i = 1; i <= new_fds.fd_count; i++)
		{
			sock_fd = new_fds.fd_array[i];
			if (FD_ISSET(sock_fd, &old_fds))
			{
				memset(buf, 0x00, MAX_LINE);

				//	nonblock ó��
				if (ioctlsocket(sock_fd, FIONBIO, &nonBlk))
					Error_Exit("NonBlock");

				if ((readn = recv(sock_fd, buf, MAX_LINE, 0)) <= 0) //��������
				{
					printf("POS %d ���� ����!\n", sock_fd);
					closesocket(sock_fd);
					FD_CLR(sock_fd, &new_fds);
				}
				else //�������
				{
					if (!strncmp("AT", buf, 2))
					{
						continue;
					}
					if (!strncmp("POS", buf, 3))
					{
						//send(sock_fd, buf, sizeof(buf), 0);
						printf("%s", buf);
						continue;
					}

					time_t t = time(NULL);
					tm curTime = *localtime(&t); //���� �ð� �޾ƿ�

					RecvData tmp;
					int i = 0;

					while (buf[i] != '\0')
					{
						tmp.type = buf[i];
						switch (tmp.type)
						{
						case 'A':
							DailyTypeSale[0] += 1;
							tmp.price = 1000;
							CoffeeBeanStock -= 1;
							TodayUsingCoffeBean += 1;
							break;
						case 'B':
							DailyTypeSale[1] += 1;
							tmp.price = 2000;
							CoffeeBeanStock -= 1;
							MilkStock -= 1;
							TodayUsingMilk += 1;
							break;
						case 'C':
							DailyTypeSale[2] += 1;
							tmp.price = 3000;
							FruitStock -= 1;
							TodayUsingFruit += 1;
							break;
						}
						tmp.date = curTime;

						//�Ϸ� ��ü �����Ϳ� �߰�
						AddRecvData(tmp);
						printf("\n\n[%d]: %c, %d\n", TotalArrIdx - 1, TotalDataArr[TotalArrIdx - 1].type, TotalDataArr[TotalArrIdx - 1].price);

						//�ֹ� ���� ������ ���� �ʿ����� ��� üũ
						ChkOrderItem();

						i++;
					}


					//chkTestCase(); // ���� Ȯ�ο� �Լ�
				}
				if (--fd_num <= 0) break;
			}
		}
	}

	closesocket(listen_fd);
	WSACleanup();
	return 0;
}