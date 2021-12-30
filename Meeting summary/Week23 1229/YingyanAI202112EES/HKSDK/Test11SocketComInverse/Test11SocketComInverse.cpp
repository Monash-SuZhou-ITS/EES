// Test11SocketComInverse.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>  
#include <iostream>
using namespace std;
#include <Winsock2.h>  
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

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

#define ZOOM_IN 11      //焦距变大（倍率变大）
#define ZOOM_OUT 12     //焦距变小（倍率变小）
#define TILT_UP 21      //云台上仰
#define TILT_DOWN 22    //云台下俯
#define PAN_LEFT 23     //云台左转
#define PAN_RIGHT 24    //云台右转
#define UP_LEFT 25      //云台上仰和左转
#define UP_RIGHT 26     //云台上仰和右转
#define DOWN_LEFT 27    //云台下俯和左转
#define DOWN_RIGHT 28   //云台下俯和右转
#define PAN_AUTO 29     //云台左右自动扫描

#define SPEED 1 //镜头速度，范围1-7，此处设为1

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

//DVR获取窗口句柄函数
HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound;         // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
										// WindowTitle.
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
										// WindowTitle.

	// Fetch current window title.
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	// Format a "unique" NewWindowTitle.
	wsprintf(pszNewWindowTitle, "%d/%d",
		GetTickCount(),
		GetCurrentProcessId());

	// Change current window title.
	SetConsoleTitle(pszNewWindowTitle);

	// Ensure window title has been updated.
	Sleep(40);

	// Look for NewWindowTitle.
	hwndFound = FindWindow(NULL, pszNewWindowTitle);

	// Restore original window title.
	SetConsoleTitle(pszOldWindowTitle);
	return(hwndFound);
}

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
	int ret = 1;
	while (ret != SOCKET_ERROR && ret != 0)
	{
		//向刚连接的客户端发送数据， 不能用监听套接字，而应该用accept返回的套接字 c
		char str[20] = "I am server!";
		ret = send(c, str, strlen(str), 0);//把flag置0

		//接受客户端的消息
		char buf[64] = { '\0' };
		char character = '\0';
		char* address = &character;
		ret = recv(c, buf, 64, 0); //把flag置0
		printf("%c,%c,%c,%c,%c,%c,%c,%c\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
		//cout << "recv" << inet_ntoa(addrClient.sin_addr) << ":   " << buf << endl; //inet_ntoa 转换为IP字符串   inet_ntoa只能为ipv4转换 属于低版本 不建议用
		cout << "Received: " << inet_ntop(AF_INET, (void*)&addrClient.sin_addr, address, sizeof(SOCKADDR_IN)) << ":   " << buf << endl; //inet_ntoa 转换为IP字符串
	} //对方关闭 返回0 , 错误返回 SOCKET_ERROR


	//6. 关闭套接字
	closesocket(s);

	//清理winsock环境
	WSACleanup();

	return 0;
}
*/


int main()
{
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
	char* EbtIP, EbtUser, EbtPswd;
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

	//实时预览
	//LONG IRealPlayHandle;
	//HWND hWnd = GetConsoleHwnd();     //获取窗口句柄
	//NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	//struPlayInfo.hPlayWnd = hWnd;       //需要SDK 解码时句柄设为有效值，仅取流不解码时可设为空
	//struPlayInfo.lChannel = 1;          //预览通道号
	//struPlayInfo.dwStreamType = 0;      //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	//struPlayInfo.dwLinkMode = 0;        //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
	//struPlayInfo.bBlocked = 1;          //0- 非阻塞取流，1- 阻塞取流

	//IRealPlayHandle = NET_DVR_RealPlay_V40(IUserID, &struPlayInfo, NULL, NULL);
	//printf("IRealPlayHandle = %ld\n", IRealPlayHandle);
	//if (IRealPlayHandle < 0)
	//{
	//	printf("NET_DVR_RealPlay_V40 error, %d\n", NET_DVR_GetLastError());
	//	NET_DVR_Logout(IUserID);
	//	NET_DVR_Cleanup();
	//	return 0;
	//}


	//PTZ控制开始
	//Sleep(5000);   //程序睡眠时间单位：ms


	//////////////////////////////////////////////////////////////Test11 start
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
	int ret = 1;
	while (ret != SOCKET_ERROR && ret != 0)
	{
		//向刚连接的客户端发送数据， 不能用监听套接字，而应该用accept返回的套接字 c
		char str[20] = "I am server!";
		//发送字符串
		ret = send(c, str, strlen(str), 0);//把flag置0

		//接受客户端的消息
		char buf[16] = { '\0' };
		char character = '\0';
		char* address = &character;
		//将python端传入的指令存入缓存字符数组buf
		ret = recv(c, buf, 16, 0); //把flag置0
		//打印出指令
		printf("%c,%c,%c,%c,%c,%c,%c,%c\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
		//cout << "recv" << inet_ntoa(addrClient.sin_addr) << ":   " << buf << endl; //inet_ntoa 转换为IP字符串   inet_ntoa只能为ipv4转换 属于低版本 不建议用
		cout << "Received: " << inet_ntop(AF_INET, (void*)&addrClient.sin_addr, address, sizeof(SOCKADDR_IN)) << ":   " << buf << endl; //inet_ntoa 转换为IP字符串
		//////////////////////////////////////////////////////////////Test11 next
		/*指令数组各个元素含义：	buf[0]	ENABLE
		buf[1]	PAN_LEFT			buf[2]	PAN_RIGHT
		buf[3]	TILT_UP				buf[4]	TILT_DOWN
		buf[5]	ZOOM_IN				buf[6]	ZOOM_OUT
		buf[7]	CRUISE				*/
		/*
		if (buf[0] == '0')
		{
			continue;
		}

		if (buf[1] == '1')	//控制镜头左转
		{
			LONG IPTZControl11, IPTZControl12;
			//IPTZControl11 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, PAN_LEFT, 0, 4);
			//Sleep(500);   //程序睡眠时间单位：ms
			//IPTZControl12 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, PAN_LEFT, 1, 4);
			//无预览速度控制
			IPTZControl11 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 0, 4);
			Sleep(500);   //程序睡眠时间单位：ms
			IPTZControl12 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 1, 4);
		}

		if (buf[2] == '1')	//控制镜头右转
		{
			LONG IPTZControl21, IPTZControl22;
			//IPTZControl21 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, PAN_RIGHT, 0, 4);
			//Sleep(500);   //程序睡眠时间单位：ms
			//IPTZControl22 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, PAN_RIGHT, 1, 4);
			//无预览速度控制
			IPTZControl21 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 0, 4);
			Sleep(500);   //程序睡眠时间单位：ms
			IPTZControl22 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 1, 4);
		}

		if (buf[3] == '1')	//控制镜头上仰
		{
			LONG IPTZControl31, IPTZControl32;
			//IPTZControl31 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, TILT_UP, 0, 4);
			//Sleep(500);   //程序睡眠时间单位：ms
			//IPTZControl32 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, TILT_UP, 1, 4);
			//无预览速度控制
			IPTZControl31 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_UP, 0, 4);
			Sleep(500);   //程序睡眠时间单位：ms
			IPTZControl32 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_UP, 1, 4);
		}

		if (buf[4] == '1')	//控制镜头下俯
		{
			LONG IPTZControl41, IPTZControl42;
			//IPTZControl41 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, TILT_DOWN, 0, 4);
			//Sleep(500);   //程序睡眠时间单位：ms
			//IPTZControl42 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, TILT_DOWN, 1, 4);
			//无预览速度控制
			IPTZControl41 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_DOWN, 0, 4);
			Sleep(500);   //程序睡眠时间单位：ms
			IPTZControl42 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_DOWN, 1, 4);
		}

		if (buf[5] == '1')	//控制镜头放大
		{
			LONG IPTZControl51, IPTZControl52;
			//IPTZControl51 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, ZOOM_IN, 0, 4);
			//Sleep(1000);   //程序睡眠时间单位：ms
			//IPTZControl52 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, ZOOM_IN, 1, 4);
			//无预览速度控制
			IPTZControl51 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, ZOOM_IN, 0, 4);
			Sleep(1000);   //程序睡眠时间单位：ms
			IPTZControl52 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, ZOOM_IN, 1, 4);
		}

		if (buf[6] == '1')	//控制镜头缩小
		{
			LONG IPTZControl61, IPTZControl62;
			//IPTZControl61 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, ZOOM_OUT, 0, 4);
			//Sleep(1000);   //程序睡眠时间单位：ms
			//IPTZControl62 = NET_DVR_PTZControlWithSpeed(IRealPlayHandle, ZOOM_OUT, 1, 4);
			//无预览速度控制
			IPTZControl61 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, ZOOM_OUT, 0, 4);
			Sleep(1000);   //程序睡眠时间单位：ms
			IPTZControl62 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, ZOOM_OUT, 1, 4);
		}
		*/

		//指令首位为0或7时，停止所有镜头动作
		if (buf[0] == '0' || buf[0] == '7')
		{
			LONG IPTZControl12, IPTZControl22, IPTZControl32, IPTZControl42, IPTZControl52, IPTZControl62,IPTZContro912, IPTZContro922, IPTZContro932, IPTZContro942;
	
			IPTZControl12 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 1, SPEED);
			IPTZControl22 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 1, SPEED);
			IPTZControl32 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_UP, 1, SPEED);
			IPTZControl42 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_DOWN, 1, SPEED);

			IPTZContro912 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, UP_LEFT, 1, SPEED);
			IPTZContro922 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, DOWN_LEFT, 1, SPEED);
			IPTZContro932 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, UP_RIGHT, 1, SPEED);
			IPTZContro942 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, DOWN_RIGHT, 1, SPEED);


			IPTZControl52 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, ZOOM_IN, 1, SPEED);
			IPTZControl62 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, ZOOM_OUT, 1, SPEED);
		}
		else if(buf[0] == '1' || buf[0] == '9' || buf[0] == '8')
		{
			if (buf[1] == '1')	//控制镜头左转
			{
				LONG IPTZContro211;
				IPTZContro211 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 1, SPEED);
				LONG IPTZControl11;
				IPTZControl11 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 0, SPEED);

			}

			if (buf[1] == '2')	//控制镜头右转
			{
				LONG IPTZContro212;
				IPTZContro212 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 1, SPEED);
				LONG IPTZControl21;
				IPTZControl21 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 0, SPEED);

			}

			if (buf[1] == '3')	//控制镜头上仰
			{
				LONG IPTZContro213;
				IPTZContro213 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_DOWN, 1, SPEED);
				LONG IPTZControl31;
				IPTZControl31 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_UP, 0, SPEED);

			}

			if (buf[1] == '4')	//控制镜头下俯
			{
				LONG IPTZContro214;
				IPTZContro214 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_UP, 1, SPEED);
				LONG IPTZControl41;
				IPTZControl41 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_DOWN, 0, SPEED);

			}

			if (buf[1] == '5')	//控制镜头左上
			{
				LONG IPTZContro211;
				IPTZContro211 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, DOWN_RIGHT, 1, SPEED);
				LONG IPTZControl11;
				IPTZControl11 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, UP_LEFT, 0, SPEED);

			}

			if (buf[1] == '6')	//控制镜头左下
			{
				LONG IPTZContro212;
				IPTZContro212 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, UP_RIGHT, 1, SPEED);
				LONG IPTZControl21;
				IPTZControl21 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, DOWN_LEFT, 0, SPEED);

			}

			if (buf[1] == '7')	//控制镜头右上
			{
				LONG IPTZContro213;
				IPTZContro213 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, DOWN_LEFT, 1, SPEED);
				LONG IPTZControl31;
				IPTZControl31 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, UP_RIGHT, 0, SPEED);

			}

			if (buf[1] == '8')	//控制镜头右下
			{
				LONG IPTZContro214;
				IPTZContro214 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, UP_LEFT, 1, SPEED);
				LONG IPTZControl41;
				IPTZControl41 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, DOWN_RIGHT, 0, SPEED);

			}
			if (buf[1] == '0')	//控制镜头停
			{
				LONG IPTZControl12, IPTZControl22, IPTZControl32, IPTZControl42, IPTZControl52, IPTZControl62, IPTZContro912, IPTZContro922, IPTZContro932, IPTZContro942;

				IPTZControl12 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 1, SPEED);
				IPTZControl22 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 1, SPEED);
				IPTZControl32 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_UP, 1, SPEED);
				IPTZControl42 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_DOWN, 1, SPEED);

				IPTZContro912 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, UP_LEFT, 1, SPEED);
				IPTZContro922 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, DOWN_LEFT, 1, SPEED);
				IPTZContro932 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, UP_RIGHT, 1, SPEED);
				IPTZContro942 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, DOWN_RIGHT, 1, SPEED);


				IPTZControl52 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, ZOOM_IN, 1, SPEED);
				IPTZControl62 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, ZOOM_OUT, 1, SPEED);

			}
		}
		else if (buf[0] == '2' ) //指令首位为2时，进入自动扫描模式
		{
			LONG IPTZContro311, IPTZContro312, IPTZContro313, IPTZContro314, IPTZControlscan1, IPTZControlscan2, IPTZControlscan3, IPTZControlscan4;

			while (1)
			{
				IPTZControlscan3 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 0, 2);
				Sleep(8000);
				IPTZControlscan4 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 1, 2);


				IPTZControlscan1 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 0, 2);
				Sleep(8000);
				IPTZControlscan2 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 1, 2);

				if (buf[0] != '2')
					break;
			}

			IPTZContro311 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_LEFT, 1, SPEED);
			IPTZContro312 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, PAN_RIGHT, 1, SPEED);
            IPTZContro313 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_UP, 1, SPEED);
            IPTZContro314 = NET_DVR_PTZControlWithSpeed_Other(IUserID, struDeviceInfo.byChanNum, TILT_DOWN, 1, SPEED);
			
		}


		//////////////////////////////////////////////////////////////Test11 last
	} //对方关闭 返回0 , 错误返回 SOCKET_ERROR

	//6. 关闭套接字
	closesocket(s);
	//清理winsock环境
	WSACleanup();
	//////////////////////////////////////////////////////////////Test11 end

	////关闭预览
	//NET_DVR_StopRealPlay(IRealPlayHandle);

	//注销用户
	NET_DVR_Logout(IUserID);

	//释放SDK资源
	NET_DVR_Cleanup();

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
