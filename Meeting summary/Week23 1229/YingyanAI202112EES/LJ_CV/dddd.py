import os

path = './Demo_data/xml/'

# 获取该目录下所有文件，存入列表中
f = os.listdir(path)
print(f)
n = 0
for i in f:
    # 设置旧文件名（就是路径+文件名）
    oldname = path + f[n]
    print(oldname)
    # 设置新文件名
    newname = path + 'a' + str(n + 201) + '.xml'
    # 用os模块中的rename方法对文件改名
    os.rename(oldname, newname)
    print(oldname, '======>', newname)
    n += 1
