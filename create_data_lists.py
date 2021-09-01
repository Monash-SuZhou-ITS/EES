from utils import create_data_lists

if __name__ == '__main__':
    create_data_lists(voc07_path='./image_data/Annotated_samples1/train/',
                      voc12_path='./image_data/Annotated_samples1/test/',
                      output_folder='./image_data/Annotated_samples1/output/')
