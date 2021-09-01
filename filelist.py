import os

# file_path = "./image_data/dataset/Annotations/"
# file_path = "./image_data/train/Annotations/"
# file_path = "./image_data/test/Annotations/"

# file_path = "./image_data/Annotated_samples1/Annotations/"
# file_path = "./image_data/Annotated_samples1/train/Annotations/"
# file_path = "./image_data/Annotated_samples1/test/Annotations/"

# file_path = "./image_data/Annotated_samples2/Annotations/"
# file_path = "./image_data/Annotated_samples2/train/Annotations/"
# file_path = "./image_data/Annotated_samples2/test/Annotations/"

file_path = "./image_annotated/Annotations/"

# os.listdir(file)会历遍文件夹内的文件并返回一个列表
path_list = os.listdir(file_path)
# 定义一个空列表,我不需要path_list中的后缀名
path_name=[]
# 利用循环历遍path_list列表并且利用split去掉后缀名
# for i in path_list:
#     path_name.append(i.split(".")[0])
for i in path_list:
    path_name.append(i.split(".x")[0])

# 排序一下
path_name.sort()

for file_name in path_name:
    # "a"表示以不覆盖的形式写入到文件中,当前文件夹如果没有"save.txt"会自动创建
    # with open("./image_data/dataset/names.txt", "a") as f:
    # with open("./image_data/train/trainval.txt", "a") as f:
    # with open("./image_data/test/test.txt","a") as f:
    # with open("./image_data/Annotated_samples1/names.txt", "a") as f:
    # with open("./image_data/Annotated_samples1/train/trainval.txt", "a") as f:
    # with open("./image_data/Annotated_samples1/test/test.txt","a") as f:
    # with open("./image_data/Annotated_samples2/names.txt", "a") as f:
    # with open("./image_data/Annotated_samples2/train/trainval.txt", "a") as f:
    # with open("./image_data/Annotated_samples2/test/test.txt","a") as f:
    with open("./image_annotated/list.txt", "a") as f:
        f.write(file_name + "\n")
        # print(file_name)
    f.close()
