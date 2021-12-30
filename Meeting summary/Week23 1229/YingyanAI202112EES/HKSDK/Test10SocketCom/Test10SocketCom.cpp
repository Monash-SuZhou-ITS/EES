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

#define NET_DVR_GET_PTZPOS 293    //获取IP快球PTZ参数


#include "math.h"
//PTZ读数与实际值转换函数
//P值的转换函数
int Ptrans16to10(int numsrc)
{
	double numtag = 0;
	//PTZ中，P值的读数由十六进制转化成十进制，再整除10后得到P的实际值
	//初始化余数和商数的数组
	int remainder[4] = { 0,0,0,0 };  //防止出现高位有值加错的情况
	int quotient[4] = { 999,999,999,999 };
	quotient[0] = numsrc;
	for (int i = 0; i <= 3; i++)
	{
		if (i < 3)
		{
			quotient[i + 1] = quotient[i] / 16;  //最后一步不需要再得商数
		}
		remainder[i] = quotient[i] % 16;
		numtag += remainder[i] * pow(10, i);
		//printf("%d, %d\n", quotient[i], remainder[i]);
	}
	//整除10
	numtag = round(numtag / 10);
	if (numtag >= 360)
	{
		numtag = 0;
	}
	return int(numtag);
}

//T值的转换函数
int Ttrans16to10(int numsrc)
{
	double numtag = 0;
	//PTZ中，T值的读数由十六进制转化成十进制，再除以10后四舍五入得到T的实际值
	//初始化余数和商数的数组
	int remainder[4] = { 0,0,0,0 };  //防止出现高位有值加错的情况
	int quotient[4] = { 999,999,999,999 };
	//若为仰角，则用负数处理
	if (numsrc > 10000)
		quotient[0] = numsrc - 13722;  //13722转换为10进制为3600
	else
		quotient[0] = numsrc;
	for (int i = 0; i <= 3; i++)
	{
		if (i < 3)
		{
			quotient[i + 1] = quotient[i] / 16;  //最后一步不需要再得商数
		}
		remainder[i] = quotient[i] % 16;
		numtag += remainder[i] * pow(10, i);
		//printf("%d, %d\n", quotient[i], remainder[i]);
	}
	//除以10后四舍五入（负数则五舍六入）
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

//Z值的转换函数
int Ztrans16to10(int numsrc)
{
	int numtag = 0;
	//PTZ中，Z值的读数由十六进制转化成十进制，再整除以10后得Z的实际值
	//初始化余数和商数的数组
	int remainder[3] = { 0,0,0 };  //防止出现高位有值加错的情况
	int quotient[3] = { 999,999,999 };
	quotient[0] = numsrc;
	for (int i = 0; i <= 2; i++)
	{
		if (i < 2)
		{
			quotient[i + 1] = quotient[i] / 16;  //最后一步不需要再得商数
		}
		remainder[i] = quotient[i] % 16;
		numtag += remainder[i] * pow(10, i);
		//printf("%d, %d\n", quotient[i], remainder[i]);
	}
	//整除10
	numtag = numtag / 10;
	return numtag;
}

//DVR回调函数
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void* pUser)
{
	char tempbuf[256] = { 0 };
	switch (dwType)
	{
	case EXCEPTION_RECONNECT: //预览时重连
		printf("----------reconnect-------- % d\n", time(NULL));
		break;
	default:
		break;
	}
}

/*
// cpp端作为客户端，函数形式
// C++作为客户端，发送实时数据
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
// cpp端作为服务端，函数形式
// C++作为服务端，接收指令数据
int cppService() {

	//初始化winsock2.2相关的动态库
	WSADATA wd; // 获取socket相关信息
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) { //0 表示成功
		cout << "WSAStartup error: " << WSAGetLastError() << endl;
		return 0;
	}

	//1. 创建TCP socket 流式套接字
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == s) {
		cout << "socket error :" << WSAGetLastError() << endl;
		return 0;
	}

	//2. 绑定socket到一个IP地址和端口
	sockaddr_in addr; //不建议使用sockaddr 建议用sockaddr_in
	addr.sin_family = AF_INET; // 地址族
	addr.sin_port = htons(8889);//本地端口 转网络字节序
	//addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//ip地址转网络字节序 inet_addr只能为ipv4转换 属于低版本 不建议用
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);//ip地址转网络字节序

	int len = sizeof(sockaddr_in);
	if (SOCKET_ERROR == bind(s, (sockaddr*)&addr, len)) {
		cout << "bind error: " << WSAGetLastError() << endl;
		return 0;
	}

	//3. 监听， 5 代表正在等待完成相应的TCP三次握手过程的队列长度
	if (SOCKET_ERROR == listen(s, 5)) { // 根据电脑配置设定链接数
		cout << "listen error:" << WSAGetLastError() << endl;
		return 0;
	}

	//4. 接受客户端请求，并且返回和客户端通讯的套接字
	sockaddr_in addrClient; //保存客户端IP地址端口
	memset(&addrClient, 0, sizeof(sockaddr_in));
	len = sizeof(sockaddr_in);
	SOCKET c = accept(s, (sockaddr*)&addrClient, &len); //成功返回套接字
	if (INVALID_SOCKET == c) {
		cout << "accept error:" << WSAGetLastError() << endl;
		return 0;
	}

	//5. 发送 接受消息
	int ret = 0;
	do {
		//向刚连接的客户端发送数据， 不能用监听套接字，而应该用accept返回的套接字 c
		char str[20] = "I am server!";
		ret = send(c, str, strlen(str), 0);//把flag置0

		//接受客户端的消息
		char buf[64] = { '\0' };
		char* address = new char;
		ret = recv(c, buf, 64, 0); //把flag置 0
		//cout << "recv" << inet_ntoa(addrClient.sin_addr) << ":   " << buf << endl; //inet_ntoa 转换为IP字符串   inet_ntoa只能为ipv4转换 属于低版本 不建议用
		cout << "recv : " << inet_ntop(AF_INET, (void*)&addrClient.sin_addr, address, sizeof(SOCKADDR_IN)) << ":   " << buf << endl; //inet_ntoa 转换为IP字符串
	} while (ret != SOCKET_ERROR && ret != 0); //对方关闭 返回0 , 错误返回 SOCKET_ERROR


	//6. 关闭套接字
	closesocket(s);

	//清理winsock环境
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

	
	//DVR初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//设置异常信息回调函数
	NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	//用户注册设备
	LONG IUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	//char* EbtIP, EbtUser, EbtPswd;
	//EbtIP = "10.6.99.101";
	//EbtUser = "admin";
	//EbtPswd = "Ebt123@abcd";
	//DVR登录需要网络IP、端口号、用户名和密码
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

	//获取当前PTZ位置 
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

	//注销用户
	NET_DVR_Logout(IUserID);

	//释放SDK资源
	NET_DVR_Cleanup();
	
	return 0;
}