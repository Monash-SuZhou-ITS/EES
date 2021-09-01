import os
import sys
import xml.dom.minidom
from lxml import etree
import pandas as pd
import xlsxwriter

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

def to_csv(writer, excel_data, sheet_name):
    data_df = pd.DataFrame(excel_data[1:])
    data_df.columns = excel_data[0]
    data_df.to_excel(writer, float_format='%.10f', index=None, sheet_name=sheet_name)
    print(data_df)
    worksheet = writer.sheets[sheet_name]
    # 设置列宽
    cols = "%s:%s" % ('A', chr(ord('A') + len(data_df.columns) - 1))
    worksheet.set_column(cols, 30)

# def write_excel_xlsx(excel_data):
root_path = "./image_annotated/"
# Find IDs of images in dataset
with open(os.path.join(root_path, 'list.txt')) as f:
    ids = f.read().splitlines()
count = 0
for id in ids:
    obj_path = os.path.join(root_path, 'Annotations/', id + '.xml')
    excel_data_ = read_data_from_xml(obj_path)
    excel_data_str = ""
    print(excel_data_, type(excel_data_))
    for i in range(len(excel_data_)):
        for j in range(len(excel_data_[i])):
            excel_data_str = excel_data_[i][j] + "\t"
    with open("./image_annotated/list_object.txt", "a") as f:
        f.write(excel_data_str + "\n")
        # print(file_name)
    f.close()

    # writer = pd.ExcelWriter('object.xlsx')
    # # print(excel_data_)
    # to_csv(writer, excel_data_, "object1")
    # writer.save()
    # count += 1
    # if count == 100:
    #     break

# 读取xml
# excel_data_ = read_data_from_xml("./image_annotated/Annotations/imgs_317-319_part1/object.xml")
# writer = pd.ExcelWriter('object.xlsx')
# to_csv(writer, excel_data_, "object1")
# writer.save()

# if __name__ == "__main__":
#     write_excel_xlsx(read_data_from_xml(xml_path))