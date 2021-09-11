import os
input_dir='./label/Annotations/192.168.0.61_01_20210319094001395_3_undistort_282.xml'
import xml.etree.ElementTree as ET
import xml.dom.minidom
n = 0
a = 0
b = 0
c = 0
d = 0
e = 0
f = 0
for filename in os.listdir(input_dir):
    file_path = os.path.join(input_dir, filename)
    dom = ET.parse(file_path)
    root = dom.getroot()
    for obj in root.iter('object'):  # 获取object节点中的name子节点
        #tmp_name = obj.find('name').text
        if obj.find('name').text =='r':
            n=n+1
        elif obj.find('name').text=='l':
            a=a+1
        elif obj.find('name').text=='lf':
            b=b+1
        elif obj.find('name').text=='ic':
            c=c+1
        elif obj.find('name').text=='bt':
            d=d+1
        elif obj.find('name').text=='ipr':
            e=e+1
        elif obj.find('name').text=='c':
            f=f+1
print('r类共有标注{}个，l类共有标注{}个，lf类共有标注{}个，ic类共有标注{}个,bt类共有标注{}个,ipr类共有标注{}个,c类共有标注{}个'.format(n,a,b,c,d,e,f))
print('共计标注{}个'.format(n+a+b+c+d+e+f))
