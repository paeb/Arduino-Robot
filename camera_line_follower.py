import sensor, image, time, pyb
from pyb import UART

uart = UART(3, 9600)

led1 = pyb.LED(1)
led2 = pyb.LED(2)
led3 = pyb.LED(3)

led1.on()
led2.on()
led3.on()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(True)
sensor.set_auto_whitebal(True)

clock = time.clock()

color_threshold = (38, 76, -34, -6, -16, 50)

while(True):
    clock.tick()
    img = sensor.snapshot()
    blobs = img.find_blobs([color_threshold], pixels_threshold = 1000, merge = True) # merges blobs
    # ignore blobs under 200 pixels in size
    for blob in blobs:
        img.draw_rectangle(blob.rect()) # draws rectangle on the blob
        position = blob.cx()
    # centroid value for blob
        img.draw_line(position, 0, position, 240, color = (255,0,0))
    # position drawn as a vertical line
        img.draw_line(160, 0, 160, 240)
    # line from center to top, assuming 160 is center

        error = position - 160
    # how far from center line the camera sees the green

        uart.write(str(error)+"\n")
        print(error)
