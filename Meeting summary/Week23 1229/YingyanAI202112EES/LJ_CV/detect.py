import argparse
import socket
import threading


import Track_and_position
from sys import platform

from models import *  # set ONNX_EXPORT in models.py
from utils.datasets import *
from utils.utils import *

Flag_Navigator=0  #0:close 1:buffer 2:open


def detect(save_img=False):
    cam_state =0
    Flag_plot=0
    Dir=[0,0,0,0,0,0]

    img_size = (320, 192) if ONNX_EXPORT else opt.img_size  # (320, 192) or (416, 256) or (608, 352) for (height, width)
    out, source, weights, half, view_img, save_txt = opt.output, opt.source, opt.weights, opt.half, opt.view_img, opt.save_txt
    webcam = source == '0' or source.startswith('rtsp') or source.startswith('http') or source.endswith('.txt')

    # Initialize
    device = torch_utils.select_device(device='cpu' if ONNX_EXPORT else opt.device)
    if os.path.exists(out):
        shutil.rmtree(out)  # delete output folder
    os.makedirs(out)  # make new output folder

    # Initialize model
    model = Darknet(opt.cfg, img_size)

    # Load weights
    attempt_download(weights)
    if weights.endswith('.pt'):  # pytorch format
        model.load_state_dict(torch.load(weights, map_location=device)['model'])
    else:  # darknet format
        load_darknet_weights(model, weights)

    # Second-stage classifier
    classify = False
    if classify:
        modelc = torch_utils.load_classifier(name='resnet101', n=2)  # initialize
        modelc.load_state_dict(torch.load('weights/resnet101.pt', map_location=device)['model'])  # load weights
        modelc.to(device).eval()

    # Fuse Conv2d + BatchNorm2d layers
    # model.fuse()

    # Eval mode
    model.to(device).eval()

    # Export mode
    if ONNX_EXPORT:
        model.fuse()
        img = torch.zeros((1, 3) + img_size)  # (1, 3, 320, 192)
        f = opt.weights.replace(opt.weights.split('.')[-1], 'onnx')  # *.onnx filename
        torch.onnx.export(model, img, f, verbose=False, opset_version=11)

        # Validate exported model
        import onnx
        model = onnx.load(f)  # Load the ONNX model
        onnx.checker.check_model(model)  # Check that the IR is well formed
        print(onnx.helper.printable_graph(model.graph))  # Print a human readable representation of the graph
        return

    # Half precision
    half = half and device.type != 'cpu'  # half precision only supported on CUDA
    if half:
        model.half()

    # Set Dataloader
    vid_path, vid_writer = None, None
    if webcam:
        view_img = True
        torch.backends.cudnn.benchmark = True  # set True to speed up constant image size inference
        dataset = LoadStreams(source, img_size=img_size)
    else:
        save_img = True
        dataset = LoadImages(source, img_size=img_size)

    # Get names and colors
    names = load_classes(opt.names)
    colors = [[random.randint(0, 255) for _ in range(3)] for _ in range(len(names))]

    # Run inference
    t0 = time.time()
    for path, img, im0s, vid_cap in dataset:


        img = torch.from_numpy(img).to(device)
        img = img.half() if half else img.float()  # uint8 to fp16/32
        img /= 255.0  # 0 - 255 to 0.0 - 1.0
        if img.ndimension() == 3:
            img = img.unsqueeze(0)

        # Inference
        t1 = torch_utils.time_synchronized()
        pred = model(img)[0].float() if half else model(img)[0]
        t2 = torch_utils.time_synchronized()

        # Apply NMS
        pred = non_max_suppression(pred, opt.conf_thres, opt.iou_thres, classes=opt.classes, agnostic=opt.agnostic_nms)

        # Apply Classifier
        if classify:
            pred = apply_classifier(pred, modelc, img, im0s)

        # Process detections
        for i, det in enumerate(pred):  # detections per image
            global PTZ_x
            global PTZ_y
            global PTZ_z


            if webcam:  # batch_size >= 1
                p, s, im0 = path[i], '%g: ' % i, im0s[i]
            else:
                p, s, im0 = path, '', im0s

            save_path = str(Path(out) / Path(p).name)
            s += '%gx%g ' % img.shape[2:]  # print string


            if det is  None:
                if cam_state == 0 or cam_state == 1 :
                    start0_2 = time.time()
                    cam_state = 7  # stop-nav buffer
                if cam_state == 7 :
                    if time.time() - start0_2 > 2:
                        cam_state = 2

            if det is not None and len(det):


                if (0 in det[:, 5]):
                     print("dududududu", det[:, 5])
                # Mode switch cam_state 0:Stop 1:Track 2:Navigator 9:buffer
                if cam_state == 1 and (0 not in det[:, 5]):
                    cam_state = 0

                if (0 not in det[:, 5]) :
                    if cam_state == 0:
                        start0_2 = time.time()
                        cam_state = 7  # stop-nav buffer
                    if cam_state == 7:
                        if time.time() - start0_2 > 2:
                            cam_state = 2
                else:
                    if cam_state == 2 or cam_state == 7 or cam_state == 0:
                        start2_1 = time.time()
                        cam_state = 8  # nav-track buffer
                    if cam_state == 8:
                        if time.time() - start2_1 > 2:
                            cam_state = 1



            if det is not None and len(det):



                # Rescale boxes from img_size to im0 size
                det[:, :4] = scale_coords(img.shape[2:], det[:, :4], im0.shape).round()

                # Print results
                for c in det[:, -1].unique():
                    n = (det[:, -1] == c).sum()  # detections per class
                    s += '%g %ss, ' % (n, names[int(c)])  # add to string





                #else:
                #   Flag_Navigator = 0
                x_2D=0
                y_2D=0
                x_3D=0
                y_3D=0
                z_3D=0
                # Write results
                for *xyxy, conf, cls in det:

                    if cls==0:

                        Dir,Flag_Track=Track_and_position.Track(xyxy)
                        #print(Flag_Track,Dir,Flag_Navigator)
                        print(xyxy)
                        # Solve Coordinate
                        Flag_plot = 1

                        x_2D, y_2D = Track_and_position.position_2D(PTZ_x, PTZ_y)
                        x_3D, y_3D, z_3D = Track_and_position.position_3D(xyxy, PTZ_x, PTZ_y)
                        poi_2D = str([format(x_2D, '.2f'),format(y_2D, '.2f')])
                        poi_3D = str([format(x_3D, '.2f'), format(y_3D, '.2f'), format(z_3D, '.2f')])
                        cv2.putText(im0, "2D_Coordinate:", (20, 30), 0, 0.8, (255, 0, 255), 4, )
                        cv2.putText(im0, poi_2D, (20, 60), 0, 0.8, (255, 0, 255), 4, )
                        cv2.putText(im0, "3D_Coordinate:", (20, 90), 0, 0.8, (255, 255, 0), 4, )
                        cv2.putText(im0, poi_3D, (20, 120), 0, 0.8, (255, 255, 0), 4, )
                    if save_txt:  # Write to file
                        with open(save_path + '.txt', 'a') as file:
                            file.write(('%g ' * 6 + '\n') % (*xyxy, cls, conf))

                    if save_img or view_img:  # Add bbox to image
                        label = '%s %.2f' % (names[int(cls)], conf)
                        plot_one_box(xyxy, im0, label=label, color=colors[int(cls)])



                    #2D/3D position plot
                    #if Flag_plot == 1:

                    #Flag_plot = 0


            # Print time (inference + NMS)
            print('%sDone. (%.3fs)' % (s, t2 - t1))
            print("目前的状态为", cam_state)
            print("控制指令为", Dir)
            global c_0 ,c_1  #stop L R U D LU LD RU RD

            if cam_state==1 or cam_state==8:
                c_0 = cam_state
                if Dir[0] + Dir[1] + Dir[2] + Dir[3] == 0:
                    c_1 = 0
                if Dir[0] + Dir[1] + Dir[2] + Dir[3] == 1:
                    if Dir[0] == 1: c_1 = 1
                    if Dir[1] == 1: c_1 = 2
                    if Dir[2] == 1: c_1 = 3
                    if Dir[3] == 1: c_1 = 4
                if Dir[0] + Dir[1] + Dir[2] + Dir[3] > 1:
                    if Dir[0] == 1 and Dir[2] == 1: c_1 = 5
                    if Dir[0] == 1 and Dir[3] == 1: c_1 = 6
                    if Dir[1] == 1 and Dir[2] == 1: c_1 = 7
                    if Dir[1] == 1 and Dir[3] == 1: c_1 = 8
            else:
                c_0 = cam_state
                c_1 = 0

            # Stream results
            if view_img:
                cv2.imshow(p, im0)
                if cv2.waitKey(1) == ord('q'):  # q to quit
                    raise StopIteration

            # Save results (image with detections)
            if save_img:
                if dataset.mode == 'images':
                    cv2.imwrite(save_path, im0)
                else:
                    if vid_path != save_path:  # new video
                        vid_path = save_path
                        if isinstance(vid_writer, cv2.VideoWriter):
                            vid_writer.release()  # release previous video writer

                        fps = vid_cap.get(cv2.CAP_PROP_FPS)
                        w = int(vid_cap.get(cv2.CAP_PROP_FRAME_WIDTH))
                        h = int(vid_cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
                        vid_writer = cv2.VideoWriter(save_path, cv2.VideoWriter_fourcc(*opt.fourcc), fps, (w, h))
                    vid_writer.write(im0)

    if save_txt or save_img:
        print('Results saved to %s' % os.getcwd() + os.sep + out)
        if platform == 'darwin':  # MacOS
            os.system('open ' + out + ' ' + save_path)

    print('Done. (%.3fs)' % (time.time() - t0))


if __name__ == '__main__':
    import cv2
    from queue import Queue
    PTZ_x =0
    PTZ_y =0
    PTZ_z =0

    c_0 = 0
    c_1 = 0


    ip = '10.6.99.101'
    user = 'admin'
    password = 'Ebt123@abcd'


    parser = argparse.ArgumentParser()

    parser.add_argument('--source', type=str, default='rtsp://admin:Ebt123@abcd@10.6.99.101/Streaming/Channels/2',
                        help='file/dir/URL/glob, 0 for webcam')

    parser.add_argument('--cfg', type=str, default='cfg/yolov3-tiny-3cls.cfg', help='*.cfg path')
    parser.add_argument('--names', type=str, default='Demo_data/Demo.names', help='*.names path')
    parser.add_argument('--weights', type=str, default='weights/best.pt', help='weights path')
    #parser.add_argument('--source', type=str, default='./LJ_data/Images/192.168.0.62_01_20210317143312565_1_undistort_1405.jpg', help='source')  # input file/folder, 0 for webcam
    parser.add_argument('--output', type=str, default='output', help='output folder')  # output folder
    parser.add_argument('--img-size', type=int, default=640, help='inference size (pixels)')
    parser.add_argument('--conf-thres', type=float, default=0.3, help='object confidence threshold')
    parser.add_argument('--iou-thres', type=float, default=0.1, help='IOU threshold for NMS')
    parser.add_argument('--fourcc', type=str, default='mp4v', help='output video codec (verify ffmpeg support)')
    parser.add_argument('--half', action='store_true', help='half precision FP16 inference')
    parser.add_argument('--device', default='', help='device id (i.e. 0 or 0,1) or cpu')
    parser.add_argument('--view-img', action='store_true', help='display results')
    parser.add_argument('--save-txt', action='store_true', help='save results to *.txt')
    parser.add_argument('--classes', nargs='+', type=int, help='filter by class')
    parser.add_argument('--agnostic-nms', action='store_true', help='class-agnostic NMS')
    opt = parser.parse_args()
    print(opt)

    def all():
         with torch.no_grad():
               detect()

    # python as the server
    def Tcp_sever():
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.bind(("localhost", 8888))
        server.listen(0)
        connection, address = server.accept()
        print(connection, address)
        num = 0
        while True:
            recv_str = connection.recv(1024)[0:30]
            time.sleep(0.1)
            print("PTZ parameters:")
            recv_str = recv_str.decode("ascii")
            if not recv_str:
                break
            num = num + 1
            print(recv_str, type(recv_str), num)
            global PTZ_x
            global PTZ_y
            global PTZ_z

            PTZ_x = int(recv_str[2:5])
            PTZ_y = int(recv_str[11:14])
            PTZ_z = int(recv_str[20:23])
            connection.send(bytes("RecvPTZ: %s" % recv_str, encoding="ascii"))
        connection.close()


    def Tcp_client():
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(("127.0.0.1", 8889))
        last_msg = 99999
        while True:
            meg=str(c_0)+str(c_1)

            #meg = '0000000'
            if meg != last_msg:
                last_msg = meg
                client.send(meg.encode())
                recv_data = client.recv(1024)
                if recv_data:
                    print("Received: ", recv_data.decode('gbk'))
                else:
                    print("对方已离线。。")
                    break
            time.sleep(0.1)
        client.close()
        input("enter end")



    threads = []
    t1 = threading.Thread(target=all)
    threads.append(t1)
    t2 = threading.Thread(target=Tcp_sever)
    threads.append(t2)
    t3 = threading.Thread(target=Tcp_client)
    threads.append(t3)

    with torch.no_grad():
        t1.start()
        t2.start()
        t3.start()


















