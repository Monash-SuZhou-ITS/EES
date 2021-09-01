import os
import sys
import xml.dom.minidom
from lxml import etree

def read_data_from_xml(xml_path):
    xml_content = ""
    with open(xml_path, 'rb') as f:
        xml_content = f.read()
    excel_data = [["object"]]
    xml_data = etree.XML(xml_content)
    if len(xml_data.xpath("//object")) != 0:
        anns = xml_data.xpath("//object")
    else:
        anns = xml_data.xpath("//source")
    # print(len(anns))
    for ann in anns:
        excel_row_data = []
        # print(ann.xpath("name/text()"), type(ann.xpath("name/text()")), len(ann.xpath("name/text()")))
        if len(xml_data.xpath("//object")) == 0:
            excel_row_data.extend(["none"])
        else:
            excel_row_data.extend(ann.xpath("name/text()"))
        excel_data.append(excel_row_data)
    return excel_data

root_path = "./image_annotated/"
# Find IDs of images in dataset
with open(os.path.join(root_path, 'list.txt')) as f:
    files = f.read().splitlines()
count = 0
for id in range(len(files)):
    obj_path = os.path.join(root_path, 'Annotations/', files[id] + '.xml')
    excel_data_ = read_data_from_xml(obj_path)
    print(files[id])
    excel_data_str = files[id] + "\t"
    #print(excel_data_, type(excel_data_))
    for i in range(1,len(excel_data_)):
        for j in range(len(excel_data_[i])):
            excel_data_str = excel_data_str + excel_data_[i][j] + "\t"
    with open("./image_annotated/list_object.txt", "a") as f:
        f.write(excel_data_str + "\n")
    f.close()
