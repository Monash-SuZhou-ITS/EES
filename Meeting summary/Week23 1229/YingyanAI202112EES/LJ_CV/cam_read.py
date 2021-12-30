#!/usr/bin/python3
# -*- coding: utf-8 -*-
# @Time : 2021/11/2 13:59
# @File : Demo1.py
# @Software: PyCharm


import cv2

ip = '10.6.99.101'
user = 'admin'
password = 'Ebt123@abcd'
cap = cv2.VideoCapture("rtsp://admin:Ebt123@abcd@10.6.99.101/Streaming/Channels/2")
ret, frame = cap.read()
cv2.namedWindow(ip, 0)
cv2.resizeWindow(ip,500,300)
while ret:
    ret, frame = cap.read()
    cv2.imshow(ip,frame)
    print(type(frame))
    # 按下q键关闭窗体
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cv2.destroyAllWindows()
cap.release()
