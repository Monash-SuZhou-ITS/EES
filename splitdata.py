# *_*coding: utf-8 *_*
# Author --LiMing--

import os
import random
import shutil
import time


def copyFile(fileDir, origion_path1, class_name):
    name = class_name
    path = origion_path1
    image_list = os.listdir(fileDir)  # 获取图片的原始路径
    image_number = len(image_list)
    train_number = int(image_number * train_rate)
    train_sample = random.sample(image_list, train_number)  # 从image_list中随机获取0.75比例的图像.
    test_sample = list(set(image_list) - set(train_sample))
    sample = [train_sample, test_sample]

    # 复制图像到目标文件夹
    for k in range(len(save_dir)):
        if os.path.isdir(save_dir[k]) and os.path.isdir(save_dir1[k]):
            for name in sample[k]:
                name1 = name.split(".")[0] + '.xml'
                shutil.copy(os.path.join(fileDir, name), os.path.join(save_dir[k], name))
                shutil.copy(os.path.join(path, name1), os.path.join(save_dir1[k], name1))
        else:
            os.makedirs(save_dir[k])
            os.makedirs(save_dir1[k])
            for name in sample[k]:
                name1 = name.split(".")[0] + '.xml'
                shutil.copy(os.path.join(fileDir, name), os.path.join(save_dir[k], name))
                shutil.copy(os.path.join(path, name1), os.path.join(save_dir1[k], name1))


if __name__ == '__main__':
    time_start = time.time()

    # 原始数据集路径
    origion_path = './image_data/dataset/JPEGImages/'
    origion_path1 = './image_data/dataset/Annotations/'

    # 保存路径
    save_train_dir = './image_data/train/JPEGImages/'
    save_test_dir = './image_data/test/JPEGImages/'
    save_train_dir1 = './image_data/train/Annotations/'
    save_test_dir1 = './image_data/test/Annotations/'
    save_dir = [save_train_dir, save_test_dir]
    save_dir1 = [save_train_dir1, save_test_dir1]

    # 训练集比例
    train_rate = 0.7

    # 数据集类别及数量
    file_list = os.listdir(origion_path)
    num_classes = len(file_list)
    for i in range(num_classes):
        class_name = file_list[i]
    copyFile(origion_path, origion_path1, class_name)
    print('划分完毕!')
    time_end = time.time()
    print('---------------')
    print('训练集和测试集划分共耗时%s!' % (time_end - time_start))
