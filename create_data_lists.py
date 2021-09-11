from utils import create_data_lists

if __name__ == '__main__':
    #create_data_lists(voc07_path='/宸哥/科研/实践项目/a-PyTorch-Tutorial-to-Object-Detection-master/media/ssd/ssd data/VOC2007',
    #                  voc12_path='/宸哥/科研/实践项目/a-PyTorch-Tutorial-to-Object-Detection-master/media/ssd/ssd data/VOC2012',
    #                  output_folder='/宸哥/科研/实践项目/a-PyTorch-Tutorial-to-Object-Detection-master/output_list')
    create_data_lists(voc07_path='./subset1000',
                      Test_path='./Test_data',
                      output_folder='./Train_output_list')