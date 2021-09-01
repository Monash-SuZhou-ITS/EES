# *_*coding: utf-8 *_*
# Author --LiMing--

import os
import random
import shutil
import time


if __name__ == '__main__':
    time_start = time.time()

    base_path = './image_annotated/'
    # 原始数据集路径
    annotation_path = os.path.join(base_path, 'Annotations/')
    image_path = os.path.join(base_path, 'JPEGImages/')

    # 各目标数据集保存路径
    save_closedgrab_annotation_dir = os.path.join(base_path, 'obj_closedgrab/Annotations/')
    save_closedgrab_image_dir = os.path.join(base_path, 'obj_closedgrab/JPEGImages/')

    save_openedgrab_annotation_dir = os.path.join(base_path, 'obj_openedgrab/Annotations/')
    save_openedgrab_image_dir = os.path.join(base_path, 'obj_openedgrab/JPEGImages/')

    save_truck_annotation_dir = os.path.join(base_path, 'obj_truck/Annotations/')
    save_truck_image_dir = os.path.join(base_path, 'obj_truck/JPEGImages/')

    save_liftedtruck_annotation_dir = os.path.join(base_path, 'obj_liftedtruck/Annotations/')
    save_liftedtruck_image_dir = os.path.join(base_path, 'obj_liftedtruck/JPEGImages/')

    save_lowdust_annotation_dir = os.path.join(base_path, 'obj_lowdust/Annotations/')
    save_lowdust_image_dir = os.path.join(base_path, 'obj_lowdust/JPEGImages/')

    save_mediumdust_annotation_dir = os.path.join(base_path, 'obj_mediumdust/Annotations/')
    save_mediumdust_image_dir = os.path.join(base_path, 'obj_mediumdust/JPEGImages/')

    save_highdust_annotation_dir = os.path.join(base_path, 'obj_highdust/Annotations/')
    save_highdust_image_dir = os.path.join(base_path, 'obj_highdust/JPEGImages/')

    save_none_annotation_dir = os.path.join(base_path, 'obj_none/Annotations/')
    save_none_image_dir = os.path.join(base_path, 'obj_none/JPEGImages/')

    save_annotation_dir = [save_closedgrab_annotation_dir, save_openedgrab_annotation_dir, save_truck_annotation_dir, save_liftedtruck_annotation_dir, save_lowdust_annotation_dir, save_mediumdust_annotation_dir, save_highdust_annotation_dir, save_none_annotation_dir]
    save_image_dir = [save_closedgrab_image_dir, save_openedgrab_image_dir, save_truck_image_dir, save_liftedtruck_image_dir, save_lowdust_image_dir, save_mediumdust_image_dir, save_highdust_image_dir, save_none_image_dir]

    # 创建保存目录
    for dir_idx in range(len(save_image_dir)):
        if os.path.isdir(save_annotation_dir[dir_idx]) and os.path.isdir(save_image_dir[dir_idx]):
            continue
        else:
            os.makedirs(save_annotation_dir[dir_idx])
            os.makedirs(save_image_dir[dir_idx])

    # 文件及其中目标的列表
    object_list_file = os.path.join(base_path, 'list_object.txt')
    with open(object_list_file) as object_list:
        objects = object_list.readlines()
    # for object in objects:
    #     print(object.rstrip())
    # print(objects)
    num_image = len(objects)
    # 遍历所有文件，如果有某类目标就保存到对应目录
    for i in range(num_image):
        object_in_image = objects[i].split('\t')
        annotation_object = object_in_image[0] + '.xml'
        image_object = object_in_image[0] + '.jpg'
        if '\tclosed grab' in objects[i]:
            shutil.copy(os.path.join(annotation_path, annotation_object) , os.path.join(save_closedgrab_annotation_dir, annotation_object))
            shutil.copy(os.path.join(image_path, image_object) , os.path.join(save_closedgrab_image_dir, image_object))
        if '\topened grab' in objects[i]:
            shutil.copy(os.path.join(annotation_path, annotation_object) , os.path.join(save_openedgrab_annotation_dir, annotation_object))
            shutil.copy(os.path.join(image_path, image_object) , os.path.join(save_openedgrab_image_dir, image_object))
        if '\ttruck' in objects[i]:
            shutil.copy(os.path.join(annotation_path, annotation_object) , os.path.join(save_truck_annotation_dir, annotation_object))
            shutil.copy(os.path.join(image_path, image_object) , os.path.join(save_truck_image_dir, image_object))
        if '\tlifted truck' in objects[i]:
            shutil.copy(os.path.join(annotation_path, annotation_object) , os.path.join(save_liftedtruck_annotation_dir, annotation_object))
            shutil.copy(os.path.join(image_path, image_object) , os.path.join(save_liftedtruck_image_dir, image_object))
        if '\tlow dust' in objects[i]:
            shutil.copy(os.path.join(annotation_path, annotation_object) , os.path.join(save_lowdust_annotation_dir, annotation_object))
            shutil.copy(os.path.join(image_path, image_object) , os.path.join(save_lowdust_image_dir, image_object))
        if '\tmedium dust' in objects[i]:
            shutil.copy(os.path.join(annotation_path, annotation_object) , os.path.join(save_mediumdust_annotation_dir, annotation_object))
            shutil.copy(os.path.join(image_path, image_object) , os.path.join(save_mediumdust_image_dir, image_object))
        if '\thigh dust' in objects[i]:
            shutil.copy(os.path.join(annotation_path, annotation_object) , os.path.join(save_highdust_annotation_dir, annotation_object))
            shutil.copy(os.path.join(image_path, image_object) , os.path.join(save_highdust_image_dir, image_object))
        if '\tnone' in objects[i]:
            shutil.copy(os.path.join(annotation_path, annotation_object) , os.path.join(save_none_annotation_dir, annotation_object))
            shutil.copy(os.path.join(image_path, image_object) , os.path.join(save_none_image_dir, image_object))
    time_end = time.time()
    print('耗时%s' % (time_end - time_start))
    # copyFile(image_path, annotation_path, class_name)
    # print('划分完毕!')
    # time_end = time.time()
    # print('---------------')
    # print('训练集和测试集划分共耗时%s!' % (time_end - time_start))
