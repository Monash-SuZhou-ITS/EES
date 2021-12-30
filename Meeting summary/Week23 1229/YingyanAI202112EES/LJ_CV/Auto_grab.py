import time
import numpy as np
import time
from PIL import ImageGrab
from matplotlib import pyplot as plt


time.time()
def screen_shot():
    img = ImageGrab.grab(bbox=(35, 270, 1400, 1250))
    img = np.array(img.getdata(), np.uint8).reshape(img.size[1], img.size[0], 3)
    plt.imshow(img, interpolation='nearest')
    tt = time.time()
    plt.imsave('./Demo_data/'+str(tt)+'.jpg', img)

i=0
while i<=10:
    screen_shot()
    time.sleep(3)
    i+=1
    print(i)




