// Test10SocketCom.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>  
#include <iostream>
using namespace std;
#include <Winsock2.h>  
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#include <string>


//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <iostream>
#include "Windows.h" 
#include "string.h" 
#include "HCNetSDK.h" 
#include "DataType.h" 
#include "DecodeCardSdk.h"
#include "plaympeg4.h" 
#include "time.h"

#define NET_DVR_GET_PTZPOS 293    //��ȡIP����PTZ����


#include "math.h"
//PTZ������ʵ��ֵת������
//Pֵ��ת������
int Ptrans16to10(int numsrc)
{
	double numtag = 0;
	//PTZ�У�Pֵ�Ķ�����ʮ������ת����ʮ���ƣ�������10��õ�P��ʵ��ֵ
	//��ʼ������������������
	int remainder[4] = { 0,0,0,0 };  //��ֹ���ָ�λ��ֵ�Ӵ�����
	int quotient[4] = { 999,999,999,999 };
	quotient[0] = numsrc;
	for (int i = 0; i <= 3; i++)
	{
		if (i < 3)
		{
			quotient[i + 1] = quotient[i] / 16;  //���һ������Ҫ�ٵ�����
		}
		remainder[i] = quotient[i] % 16;
		numtag += remainder[i] * pow(10, i);
		//printf("%d, %d\n", quotient[i], remainder[i]);
	}
	//����10
	numtag = round(numtag / 10);
	if (numtag >= 360)
	{
		numtag = 0;
	}
	return int(numtag);
}

//Tֵ��ת������
int Ttrans16to10(int numsrc)
{
	double numtag = 0;
	//PTZ�У�Tֵ�Ķ�����ʮ������ת����ʮ���ƣ��ٳ���10����������õ�T��ʵ��ֵ
	//��ʼ������������������
	int remainder[4] = { 0,0,0,0 };  //��ֹ���ָ�λ��ֵ�Ӵ�����
	int quotient[4] = { 999,999,999,999 };
	//��Ϊ���ǣ����ø�������
	if (numsrc > 10000)
		quotient[0] = numsrc - 13722;  //13722ת��Ϊ10����Ϊ3600
	else
		quotient[0] = numsrc;
	for (int i = 0; i <= 3; i++)
	{
		if (i < 3)
		{
			quotient[i + 1] = quotient[i] / 16;  //���һ������Ҫ�ٵ�����
		}
		remainder[i] = quotient[i] % 16;
		numtag += remainder[i] * pow(10, i);
		//printf("%d, %d\n", quotient[i], remainder[i]);
	}
	//����10���������루�������������룩
	int numtagq;
	numtagq = int(numtag) % 10;
	if (numtagq == -5)
	{
		numtag = round(numtag / 10) + 1;
	}
	else
	{
		numtag = round(numtag / 10);
	}
	return int(numtag);
}

//Zֵ��ת������
int Ztrans16to10(int numsrc)
{
	int numtag = 0;
	//PTZ�У�Zֵ�Ķ�����ʮ������ת����ʮ���ƣ���������10���Z��ʵ��ֵ
	//��ʼ������������������
	int remainder[3] = { 0,0,0 };  //��ֹ���ָ�λ��ֵ�Ӵ�����
	int quotient[3] = { 999,999,999 };
	quotient[0] = numsrc;
	for (int i = 0; i <= 2; i++)
	{
		if (i < 2)
		{
			quotient[i + 1] = quotient[i] / 16;  //���һ������Ҫ�ٵ�����
		}
		remainder[i] = quotient[i] % 16;
		numtag += remainder[i] * pow(10, i);
		//printf("%d, %d\n", quotient[i], remainder[i]);
	}
	//����10
	numtag = numtag / 10;
	return numtag;
}

//DVR�ص�����
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void* pUser)
{
	char tempbuf[256] = { 0 };
	switch (dwType)
	{
	case EXCEPTION_RECONNECT: //Ԥ��ʱ����
		printf("----------reconnect-------- % d\n", time(NULL));
		break;
	default:
		break;
	}
}

/*
// cpp����Ϊ�ͻ��ˣ�������ʽ
// C++��Ϊ�ͻ��ˣ�����ʵʱ����
int cppClient(char *p)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return -1;
	}
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//char strings[] = "hello";
	char* baseCh = p;

	while (true)
	{
		send(sockClient, baseCh, strlen(baseCh) + 1, 0);
		char recvBuf[40] = {'\0'};
		recv(sockClient, recvBuf, 40, 0);
		recvBuf[39] = '\0';
		printf("%s\n", recvBuf);
		//Sleep(500);
		//char t[2];
		//std::cin >> t;
	}

	closesocket(sockClient);
	WSACleanup();

	getchar();
	return 0;
}
*/

/*
// cpp����Ϊ����ˣ�������ʽ
// C++��Ϊ����ˣ�����ָ������
int cppService() {

	//��ʼ��winsock2.2��صĶ�̬��
	WSADATA wd; // ��ȡsocket�����Ϣ
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) { //0 ��ʾ�ɹ�
		cout << "WSAStartup error: " << WSAGetLastError() << endl;
		return 0;
	}

	//1. ����TCP socket ��ʽ�׽���
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == s) {
		cout << "socket error :" << WSAGetLastError() << endl;
		return 0;
	}

	//2. ��socket��һ��IP��ַ�Ͷ˿�
	sockaddr_in addr; //������ʹ��sockaddr ������sockaddr_in
	addr.sin_family = AF_INET; // ��ַ��
	addr.sin_port = htons(8889);//���ض˿� ת�����ֽ���
	//addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//ip��ַת�����ֽ��� inet_addrֻ��Ϊipv4ת�� ���ڵͰ汾 ��������
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//ip��ַת�����ֽ���

	int len = sizeof(sockaddr_in);
	if (SOCKET_ERROR == bind(s, (sockaddr*)&addr, len)) {
		cout << "bind error: " << WSAGetLastError() << endl;
		return 0;
	}

	//3. ������ 5 �������ڵȴ������Ӧ��TCP�������ֹ��̵Ķ��г���
	if (SOCKET_ERROR == listen(s, 5)) { // ���ݵ��������趨������
		cout << "listen error:" << WSAGetLastError() << endl;
		return 0;
	}

	//4. ���ܿͻ������󣬲��ҷ��غͿͻ���ͨѶ���׽���
	sockaddr_in addrClient; //����ͻ���IP��ַ�˿�
	memset(&addrClient, 0, sizeof(sockaddr_in));
	len = sizeof(sockaddr_in);
	SOCKET c = accept(s, (sockaddr*)&addrClient, &len); //�ɹ������׽���
	if (INVALID_SOCKET == c) {
		cout << "accept error:" << WSAGetLastError() << endl;
		return 0;
	}

	//5. ���� ������Ϣ
	int ret = 0;
	do {
		//������ӵĿͻ��˷������ݣ� �����ü����׽��֣���Ӧ����accept���ص��׽��� c
		char str[20] = "I am server!";
		ret = send(c, str, strlen(str), 0);//��flag��0

		//���ܿͻ��˵���Ϣ
		char buf[64] = { '\0' };
		char* address = new char;
		ret = recv(c, buf, 64, 0); //��flag�� 0
		//cout << "recv" << inet_ntoa(addrClient.sin_addr) << ":   " << buf << endl; //inet_ntoa ת��ΪIP�ַ���   inet_ntoaֻ��Ϊipv4ת�� ���ڵͰ汾 ��������
		cout << "recv : " << inet_ntop(AF_INET, (void*)&addrClient.sin_addr, address, sizeof(SOCKADDR_IN)) << ":   " << buf << endl; //inet_ntoa ת��ΪIP�ַ���
	} while (ret != SOCKET_ERROR && ret != 0); //�Է��ر� ����0 , ���󷵻� SOCKET_ERROR


	//6. �ر��׽���
	closesocket(s);

	//����winsock����
	WSACleanup();

	return 0;
}
*/

int main()
{
	//char datastr[] = "P:052     T:-20     Z:25     \n";
	//char *strPoint;
	//strPoint = datastr;
	//cppClient(strPoint);
	//cppService();

	
	//DVR��ʼ��
	NET_DVR_Init();
	//��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//�����쳣��Ϣ�ص�����
	NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	//�û�ע���豸
	LONG IUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	//char* EbtIP, EbtUser, EbtPswd;
	//EbtIP = "10.6.99.101";
	//EbtUser = "admin";
	//EbtPswd = "Ebt123@abcd";
	//DVR��¼��Ҫ����IP���˿ںš��û���������
	IUserID = NET_DVR_Login_V30("10.6.99.101", 8000, "admin", "Ebt123@abcd", &struDeviceInfo);
	printf("IUserID = %ld\n", IUserID);
	if (IUserID < 0)
	{
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return 0;
	}

	//////////////////////////////////////////////////////////////
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return -1;
	}
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8888);
	connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	char* baseCh;
	//////////////////////////////////////////////////////////////

	//��ȡ��ǰPTZλ�� 
	int iPTZ;
	NET_DVR_PTZPOS struPTZParams = { 0 };
	DWORD dwPTZReturnLen;
	while (1)
	{
		iPTZ = NET_DVR_GetDVRConfig(IUserID, NET_DVR_GET_PTZPOS, struDeviceInfo.byStartChan, &struPTZParams, sizeof(NET_DVR_PTZPOS), &dwPTZReturnLen);
		if (!iPTZ)
		{
			printf("NET_DVR_GetDVRConfig NET_DVR_PTZPOS error.\n");
			NET_DVR_Logout(IUserID);
			NET_DVR_Cleanup();
			return 0;
		}


		char datastr[50];
		sprintf(datastr, "P:%3d    T:%3d    Z:%3d    \n", Ptrans16to10(struPTZParams.wPanPos), Ttrans16to10(struPTZParams.wTiltPos), Ztrans16to10(struPTZParams.wZoomPos));
		char *strPoint;
		strPoint = datastr;
		//cppClient(strPoint);
		//////////////////////////////////////////////////////////////
		baseCh = datastr;
		send(sockClient, baseCh, strlen(baseCh) + 1, 0);
		char recvBuf[40] = { '\0' };
		recv(sockClient, recvBuf, 40, 0);
		recvBuf[39] = '\0';
		printf("%s\n", recvBuf);
		//////////////////////////////////////////////////////////////
		//printf("P:%d  (%d)    T:%d  (%d)    Z:%d  (%d)\n", Ptrans16to10(struPTZParams.wPanPos), struPTZParams.wPanPos, Ttrans16to10(struPTZParams.wTiltPos), struPTZParams.wTiltPos, Ztrans16to10(struPTZParams.wZoomPos), struPTZParams.wZoomPos);
		Sleep(100);
	}

	//////////////////////////////////////////////////////////////
	closesocket(sockClient);
	WSACleanup();

	getchar();
	//////////////////////////////////////////////////////////////

	//ע���û�
	NET_DVR_Logout(IUserID);

	//�ͷ�SDK��Դ
	NET_DVR_Cleanup();
	
	return 0;
}