from xml.dom.minidom import parse
import xml.dom.minidom 
import os
import sys
import xlsxwriter

path = './image_data/dataset/'

def readxml(name):
    #rootpath = './image_data/dataset/Annotations/'
    path = name
    dom = xml.dom.minidom.parse(path)
    testResults = dom.documentElement
    requestlist = [['用例名称','请求入参','请求出参']]
    if testResults.hasAttribute("version"):
        print("测试版本 : %s" % testResults.getAttribute("version"))
        requestlist.insert(0,testResults.getAttribute("version"))
    testCases=testResults.getElementsByTagName('httpSample')
    for testCase in testCases:
        # if testCase.hasAttribute('lb'):
            # print ("用例名称: %s" % testCase.getAttribute("lb"))
        request = testCase.getElementsByTagName('queryString')
        # print("请求入参： %s" % request[0].childNodes[0].data)
        response = testCase.getElementsByTagName('responseData')
        # print("请求出参： %s" % response[0].childNodes[0].data)
        requestlist.append([testCase.getAttribute("lb"),request[0].childNodes[0].data,response[0].childNodes[0].data])
    return requestlist

def write_excel_xlsx(value):
    #rootpath = os.getcwd()
    path = value
    newxlsx = xlsxwriter.Workbook(path)
    newsheet = newxlsx.add_worksheet()
    for i in range(1,len(value)):
        for j in range(len(value[1])):
            newsheet.write(i-1,j,str(value[i][j]))
    newxlsx.close()
    print("xlsx格式表格写入数据成功！")

if __name__ == "__main__":
    with open(os.path.join(path, 'names.txt')) as f:
        ids = f.read().splitlines()

    for id in ids:
        # Parse annotation's XML file
        objects = os.path.join(path, 'Annotations/', id + '.xml')
        write_excel_xlsx(readxml(objects))
