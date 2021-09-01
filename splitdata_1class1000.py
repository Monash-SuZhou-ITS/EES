# *_*coding: utf-8 *_*
# Author --LiMing--

import os
import random
import shutil
import time
import math


def copyFile(fileDir, origin_path1, class_name):
    #name = class_name
    path = origin_path1
    image_list = os.listdir(fileDir)  # 获取图片的原始路径
    image_number = len(image_list)
    # 抽取的固定数量
    extract_number = 1000  # 每类图片均抽1000张，每类原始图片数量超过1000张的直接随机抽取，不够1000张的复制多遍+余下名额再抽取

    # 复制图像到目标文件夹
    # n_i < 1000
    if image_number < extract_number:
        extract_wholy_times = extract_number // image_number
        extract_rest_sample = random.sample(image_list, extract_number % image_number)
        for i_times in range(extract_wholy_times):
            if os.path.isdir(save_dir) and os.path.isdir(save_dir1):
                for name in image_list:
                    name1 = name.split(".j")[0] + '.xml'
                    # 重命名防止多次执行覆盖
                    new_name = name.split(".j")[0] + '___' + str(i_times) + '.jpg'
                    new_name1 = name.split(".j")[0] + '___' + str(i_times) + '.xml'
                    shutil.copy(os.path.join(fileDir, name), os.path.join(save_dir, new_name))
                    shutil.copy(os.path.join(path, name1), os.path.join(save_dir1, new_name1))
            else:
                os.makedirs(save_dir)
                os.makedirs(save_dir1)
                for name in image_list:
                    name1 = name.split(".j")[0] + '.xml'
                    new_name = name.split(".j")[0] + '___' + str(i_times) + '.jpg'
                    new_name1 = name.split(".j")[0] + '___' + str(i_times) + '.xml'
                    shutil.copy(os.path.join(fileDir, name), os.path.join(save_dir, new_name))
                    shutil.copy(os.path.join(path, name1), os.path.join(save_dir1, new_name1))
        # 余下随机抽取
        for name in extract_rest_sample:
            name1 = name.split(".j")[0] + '.xml'
            shutil.copy(os.path.join(fileDir, name), os.path.join(save_dir, name))
            shutil.copy(os.path.join(path, name1), os.path.join(save_dir1, name1))
    # n_i >=1000
    else:
        extract_sample = random.sample(image_list, extract_number)
        # 随机抽取
        if os.path.isdir(save_dir) and os.path.isdir(save_dir1):
            for name in extract_sample:
                name1 = name.split(".j")[0] + '.xml'
                shutil.copy(os.path.join(fileDir, name), os.path.join(save_dir, name))
                shutil.copy(os.path.join(path, name1), os.path.join(save_dir1, name1))
        else:
            os.makedirs(save_dir)
            os.makedirs(save_dir1)
            for name in extract_sample:
                name1 = name.split(".j")[0] + '.xml'
                shutil.copy(os.path.join(fileDir, name), os.path.join(save_dir, name))
                shutil.copy(os.path.join(path, name1), os.path.join(save_dir1, name1))


if __name__ == '__main__':
    time_start = time.time()

    base_path = './image_annotated/'

    # 原始数据集路径
    # origin_path = os.path.join(base_path, 'obj_closedgrab/JPEGImages/')
    # origin_path1 = os.path.join(base_path, 'obj_closedgrab/Annotations/')
    # origin_path = os.path.join(base_path, 'obj_openedgrab/JPEGImages/')
    # origin_path1 = os.path.join(base_path, 'obj_openedgrab/Annotations/')
    # origin_path = os.path.join(base_path, 'obj_truck/JPEGImages/')
    # origin_path1 = os.path.join(base_path, 'obj_truck/Annotations/')
    # origin_path = os.path.join(base_path, 'obj_liftedtruck/JPEGImages/')
    # origin_path1 = os.path.join(base_path, 'obj_liftedtruck/Annotations/')
    # origin_path = os.path.join(base_path, 'obj_lowdust/JPEGImages/')
    # origin_path1 = os.path.join(base_path, 'obj_lowdust/Annotations/')
    # origin_path = os.path.join(base_path, 'obj_mediumdust/JPEGImages/')
    # origin_path1 = os.path.join(base_path, 'obj_mediumdust/Annotations/')
    # origin_path = os.path.join(base_path, 'obj_highdust/JPEGImages/')
    # origin_path1 = os.path.join(base_path, 'obj_highdust/Annotations/')
    origin_path = os.path.join(base_path, 'obj_none/JPEGImages/')
    origin_path1 = os.path.join(base_path, 'obj_none/Annotations/')
    # 保存路径
    # save_dir = os.path.join(base_path, 'obj_closedgrab/subset1000/JPEGImages/')
    # save_dir1 = os.path.join(base_path, 'obj_closedgrab/subset1000/Annotations/')
    # save_dir = os.path.join(base_path, 'obj_openedgrab/subset1000/JPEGImages/')
    # save_dir1 = os.path.join(base_path, 'obj_openedgrab/subset1000/Annotations/')
    # save_dir = os.path.join(base_path, 'obj_truck/subset1000/JPEGImages/')
    # save_dir1 = os.path.join(base_path, 'obj_truck/subset1000/Annotations/')
    # save_dir = os.path.join(base_path, 'obj_liftedtruck/subset1000/JPEGImages/')
    # save_dir1 = os.path.join(base_path, 'obj_liftedtruck/subset1000/Annotations/')
    # save_dir = os.path.join(base_path, 'obj_lowdust/subset1000/JPEGImages/')
    # save_dir1 = os.path.join(base_path, 'obj_lowdust/subset1000/Annotations/')
    # save_dir = os.path.join(base_path, 'obj_mediumdust/subset1000/JPEGImages/')
    # save_dir1 = os.path.join(base_path, 'obj_mediumdust/subset1000/Annotations/')
    # save_dir = os.path.join(base_path, 'obj_highdust/subset1000/JPEGImages/')
    # save_dir1 = os.path.join(base_path, 'obj_highdust/subset1000/Annotations/')
    save_dir = os.path.join(base_path, 'obj_none/subset1000/JPEGImages/')
    save_dir1 = os.path.join(base_path, 'obj_none/subset1000/Annotations/')


    # 数据集类别及数量
    file_list = os.listdir(origin_path)
    num_classes = len(file_list)
    for i in range(num_classes):
        class_name = file_list[i]
    copyFile(origin_path, origin_path1, class_name)
    print('抽取完毕!')
    time_end = time.time()
    print('---------------')
    print('抽取子数据集共耗时%s!' % (time_end - time_start))
