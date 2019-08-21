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

//	�����Լ� ������� �� ����
void Error_Exit(char* str)
{
	printf("%s Error!! Exit...\n", str);
	_getch();
	exit(1);
}

/*void chkTestCase() // ���� Ȯ�ο� �Լ�
{
	printf("chkTestCase ȣ��� TotalArrIdx: %d!\n", TotalArrIdx); //���� ����
	int i;
	for (i = 0; i < TotalArrIdx; i++)
	{
		printf("type: %c  price: %d  hour: %d  min: %d\n",
			TotalDataArr[i].type, TotalDataArr[i].price, TotalDataArr[i].date.tm_hour, TotalDataArr[i].date.tm_min);
	}
}*/

int main()
{
	FILE *fp;
	fp = fopen("AccountDB.txt", "w+");

	WSADATA wsaData;
	SOCKET listen_fd, accept_fd, max_fd = 0, sock_fd;
	struct sockaddr_in listen_addr, accept_addr;
	char buf[MAX_LINE];

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
		if (IsDeadline(16, 54, 0)) //(��,��,��)
		{
			struct tm tm;
			time_t t = time(NULL);
			struct tm curTime = *localtime(&t); //���� �ð� �޾ƿ�

			printf("���� �ð��Դϴ�.\n���� �Ǹž�: %d\n", CalDayAccount());
			printf("���ñ��� �̹� �� �Ǹž�: %d\n", CalMonthAccount());

			printf("�Ǹ� ������ ������ ...  ");
			fprintf(fp, "-------- %d/%d%d/ ���� ���� --------\n", curTime.tm_year + 100, curTime.tm_mon + 1, curTime.tm_mday);
			fprintf(fp, "-------- ���� �Ǹž�: %d    �̹� �� �Ǹž�: %d --------\\n\n", CalDayAccount(), CalMonthAccount());
			Sleep(1000);
			printf("�Ϸ�\n");
			DailyInit();

			for (i = 1; i <= new_fds.fd_count; i++)
			{
				sock_fd = new_fds.fd_array[i];
				if (FD_ISSET(sock_fd, &old_fds))
				{
					memset(buf, 0x00, MAX_LINE);

					//	nonblock ó��
					if (ioctlsocket(sock_fd, FIONBIO, &nonBlk))
						Error_Exit("NonBlock");

					send(sock_fd, "OFF", sizeof("OFF"), 0);
				}
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
							tmp.price = 1000;
							CoffeeBeanStock -= 1;
							break;
						case 'B':
							tmp.price = 2000;
							CoffeeBeanStock -= 1;
							MilkStock -= 1;
							break;
						case 'C':
							tmp.price = 3000;
							FruitStock -= 1;
							break;
						}
						tmp.date = curTime;

						//�Ϸ� ��ü �����Ϳ� �߰�
						AddRecvData(tmp);
						printf("totaldata[%d]: %c, %d\n", TotalArrIdx-1, TotalDataArr[TotalArrIdx - 1].type, TotalDataArr[TotalArrIdx - 1].price);

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

	fclose(fp);
	closesocket(listen_fd);
	WSACleanup();
	return 0;
}