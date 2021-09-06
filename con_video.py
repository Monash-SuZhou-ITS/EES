import numpy as np
import cv2
import glob
import os
#读取一张图片
size = (1280,720)
print(size)
#完成写入对象的创建，第一个参数是合成之后的视频的名称，第二个参数是可以使用的编码器，第三个参数是帧率即每秒钟展示多少张图片，第四个参数是图片大小信息
fourcc = cv2.VideoWriter_fourcc(*"mp4v")
videowrite = cv2.VideoWriter(r'./predict.mp4',fourcc,5,size)#20是帧数，size是图片尺寸
img_array=[]


WSI_MASK_PATH = './Predict_data/'#存放图片的文件夹路径
paths = glob.glob(os.path.join(WSI_MASK_PATH, '*.jpg'))
paths.sort()


for filename in paths:
 img = cv2.imread(filename)
 if img is None:
  print(filename + " is error!")
  continue
 img_array.append(img)
for i in range(737):
 videowrite.write(img_array[i])
 if i %100==0:
  print('all data: 737,  now load:', i)
print('end!')