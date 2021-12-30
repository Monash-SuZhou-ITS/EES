import math



Max_length=640   #the size of video (length)
Max_width=544     #the size of video (width)
Direction=[0,0,0,0]#Left Right Up Down (Initial)

CAM_P = 0  #get real time data from the ball_camera (Pitch)
CAM_T = 0  #get real time data from the ball_camera (Yaw)

ball_cam_x = 0   #The default coordinates (x) of ball_camera
ball_cam_y = 0   #The default coordinates (y) of ball_camera
ball_cam_z = 140 #The default coordinates (z) of ball_camera (cm)

tran_scale = 21000  #Scale conversion between image coordinate system and world coordinate system
                 #Need manual calculation

def deg2rad(ang):
    ang = ang * math.pi / 180
    return ang

def Track(poi):
    Direction = [0, 0, 0, 0, 0, 0]
    Actual_position_x = (poi[2]+ poi[0])/2
    Actual_position_y = (poi[3]+ poi[1])/2
    Target_position_x = Max_length / 2
    Target_position_y = Max_width / 2

    if (abs(Actual_position_x - Target_position_x)+abs(Actual_position_y - Target_position_y))>100:
        Flag_enable=1
        if abs(Actual_position_x - Target_position_x)>50:
            if Actual_position_x - Target_position_x >0:
                Direction[1] = 1
            else:
                Direction[0] = 1
        if abs(Actual_position_y - Target_position_y) > 50:
            if Actual_position_y - Target_position_y > 0:
                Direction[3] = 1
            else:
                Direction[2] = 1
    else:
        Flag_enable = 0
        Direction = [0, 0, 0, 0]

    return Direction,Flag_enable


def position_2D(P,T):
    print("PPPPPTTTTT",P,T)
    rel_pol_l = math.tan(deg2rad(90-T))*ball_cam_z
    print("llllll",rel_pol_l)
    Target_x = rel_pol_l * math.sin(deg2rad(P))+ball_cam_x
    Target_y = rel_pol_l * math.cos(deg2rad(P))+ball_cam_y
    print("xxxxxxyyyyzzzz", Target_x, Target_y)
    return Target_x,Target_y


def position_3D(poi,P,T):
    depth_est = int(tran_scale / (poi[3] + poi[2] - poi[1]- poi[0]))
    rel_pol_l = depth_est * math.sin(deg2rad(T))
    Target_z = ball_cam_z - depth_est * math.cos(deg2rad(T))
    Target_x = rel_pol_l * math.sin(deg2rad(P)) + ball_cam_x
    Target_y = rel_pol_l * math.cos(deg2rad(P)) + ball_cam_y
    return Target_x, Target_y, Target_z







