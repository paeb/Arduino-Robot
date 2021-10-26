import sensor, image, time, math, pyb
from pyb import UART

uart = UART(3, 9600)

led1 = pyb.LED(1)
led2 = pyb.LED(2)
led3 = pyb.LED(3)

led1.on()
led2.on()
led3.on()

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

clock = time.clock()
conversion = -0.8

while(True):
    clock.tick()
    img = sensor.snapshot()
    tags = img.find_apriltags(families = image.TAG16H5) # make a list of april tags
    # there are different tag families (30 in this one)
    if (len(tags) == 0):
        uart.write(str(1234) + "\n")
    else:
        for tag in tags:
            img.draw_rectangle(tag.rect())
            position = tag.cx()
            img.draw_line(position, 0, position, 120, color = (255,0,0))
            img.draw_line(80, 0, 80, 120)
            error = position - 80

            distance = tag.z_translation() / conversion

            if (distance > 4.5):
                uart.write(str(error)+"\n")
            else:
                uart.write(str(6969) + "\n")