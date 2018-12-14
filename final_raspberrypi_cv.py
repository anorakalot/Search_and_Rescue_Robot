from gpiozero import LED
import subprocess
from SimpleCV import Image
from SimpleCV improt Color

import time 


figure_here = Image("figure.png")
figure_here.show()
time.sleep(2)
red_figure = figure_here.colorDistance(Color.RED)
red_figure.show()
time.sleep(2)

only_figure = figure_here-red_figure
only_figure.show()
time.sleep(2)
figure_color_val = only_figure.meanColor()
print "color val with figure" 
print figure_color_val



figure_not_here = Image("figure.png")
figure_not_here.show()
time.sleep(2)
red_figure = figure_not_here.colorDistance(Color.RED)
red_figure.show()
time.sleep(2)

no_figure = figure_here-red_figure
no_figure.show()
time.sleep(2)
no_figure_color_val = no_figure.meanColor()
print "color val with no figure" 
print no_figure_color_val

led = LED(2)

while(1):
	subprocess.call("raspistill -n -w %s -h %s -o photo.png" % (640,480),shell=True)
	photo == Image("photo.png")
	red_figure = photo.colorDistance(Color.RED)
	only_photo_color = figure_here-red_figure
	photo_color_val = only_photo_color.meanColor()
	print "photo_color_val"
	print photo_color_val
	
	if photo_color_val[2] >= 1.12:
		led.on()
		print "figure_here!!!"
	else:
		led.off()
		print "figure_not_here"
	photo.show()
	time.sleep(2)
