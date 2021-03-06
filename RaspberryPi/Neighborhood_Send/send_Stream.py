# import the necessary packages
from imutils.video import VideoStream
import imagezmq
import socket
import time


# initialize the ImageSender object with the socket address of the
# server
sender = imagezmq.ImageSender(connect_to="tcp://192.168.1.189:5555")

# get the host name, initialize the video stream, and allow the
# camera sensor to warmup
rpiName = socket.gethostname()
#vs = VideoStream(usePiCamera=True).start()
vs = VideoStream(src=0).start()
time.sleep(2.0)
 
while True:
	# read the frame from the camera and send it to the server
	frame = vs.read()
	sender.send_image(rpiName, frame)