import socket
import time
'''
if __name__ == '__main__':
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("localhost", 8888))
    server.listen(0)
    connection, address = server.accept()
    print(connection, address)
    num = 0
    while True:
        # connection, address = server.accept()
        # print(connection, address)

        recv_str = connection.recv(1024)[0:5]
        print("enter 1")
        recv_str = recv_str.decode("ascii")
        if not recv_str:
            break
        num = num + 1
        print(recv_str, num)

        connection.send(bytes("clientRecv: %s," % recv_str, encoding="ascii"))
        time.sleep(0.5)

    connection.close()
    input("enter end")
'''


from socket import *

def main():
    # 1.创建tcp_client_socket 套接字对象
    tcp_client_socket = socket(AF_INET,SOCK_STREAM)
    # 作为客户端，主动连接服务器较多，一般不需要绑定端口

    # 2.连接服务器
    tcp_client_socket.connect(("127.0.0.1",8888))
    while True:
        """无限循环可以实现无限聊天"""
        # 3.向服务器发送数据
        meg = input("请输入要发送的消息：")
        tcp_client_socket.send(meg.encode())# 在linux中默认是utf-8编码
        # 在udp协议中使用的sendto() 因为udp发送的为数据报，包括ip port和数据，           # 所以sendto()中需要传入address，而tcp为面向连接，再发送消息之前就已经连接上了目标主机

        # 4.接收服务器返回的消息
        recv_data = tcp_client_socket.recv(1024)  # 此处与udp不同，客户端已经知道消息来自哪台服务器，不需要用recvfrom了

        if recv_data:
            print("返回的消息为:",recv_data.decode('gbk'))
        else:
            print("对方已离线。。")
            break

    tcp_client_socket.close()


if __name__ == '__main__':
    main()