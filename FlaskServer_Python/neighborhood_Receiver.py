# -*- coding: utf-8 -*-
"""
Created on Fri Feb 21 02:09:02 2020

@author: Faayed
"""



def neigh_watch(firebase, message):
    import cv2
    import imagezmq
    import time
    from firebase_admin import messaging

    

    


    # See documentation on defining a message payload.

    res = firebase.get('/neighbor',None)
    if str(res) == "Off":
        cv2.destroyAllWindows()
        return
    image_hub = imagezmq.ImageHub()
    object_classifier = cv2.CascadeClassifier("C:/Users/User/Documents/FYP/Python Folder/Flask/facial_recognition_model.xml")
    start = time.time()
    end = time.time()
    j = 0
    while True:  # show streamed images until Ctrl-C
        rpi_name, image = image_hub.recv_image()
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        objects = object_classifier.detectMultiScale(gray, 1.1, 4)
        
        if j%5 == 0:
            res = firebase.get('/neighbor',None)
            check = str(res)
            cv2.putText(image,str(res),(250,250), cv2.FONT_HERSHEY_SIMPLEX, 7,(255,255,255),10,cv2.LINE_AA)
            if check == "Off":
                cv2.destroyAllWindows()
                return
            if len(objects)>0:
                end = time.time()
            else:
                start = time.time()
                
        for (x, y, w, h) in objects:
            cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)
        j+=1
        j= j%11
        #font = cv2.FONT_HERSHEY_SIMPLEX
        #cv2.putText(image,str(j),(250,250), font, 7,(255,255,255),10,cv2.LINE_AA)
        if (end-start) >= 3:
            response = messaging.send(message)
            print('Successfully sent message:', response)
            start = time.time()
    
        cv2.imshow(rpi_name, image)  # 1 window for each RPi
        cv2.waitKey(1)
        image_hub.send_reply(b'OK')
        
    return

