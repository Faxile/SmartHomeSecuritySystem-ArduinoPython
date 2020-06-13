# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

from flask import Flask, jsonify
from pattern_Learner import find_Pred
from neighborhood_Receiver import neigh_watch
from notification import notif
import firebase_admin
from firebase_admin import credentials,db, messaging 
from firebase import firebase
from time_Now import getNow
app = Flask(__name__)


cred = ''


@app.route("/")
def home():
    return "Flask Server"

@app.route("/pattern")
def patt():
    patte = find_Pred()
    return patte

@app.route("/neighbor")
def neighbor():
    neigh_watch(firebase, message)
    return "Done :)"

@app.route("/time")
def time():
    return getNow()

@app.route("/notify")
def notify():
    return notif(patt_message)

@app.route("/cam_notify")
def cam_notify():
    return notif(cam_message)

if __name__ == "__main__":
    firebase = firebase.FirebaseApplication("https://smarthomefyp-f9244.firebaseio.com/")
    cred = credentials.Certificate("C:/Users/User/Documents/FYP/Python Folder/Flask_Updated/smart-home-firebase.json")
    default_app = firebase_admin.initialize_app(cred, {'databaseURL': 'https://smarthomefyp-f9244.firebaseio.com/'})
    topic = 'smarthometest'
    ref = db.reference('users/141/notification_token')
    registration_token = ref.get()

    message = messaging.Message(
        notification = messaging.Notification(
                title = "Suspicious Activity Detected",
                body = "We have detected suspicious activity outside your house. Please click on the Start Livestream button to see what's happening."
                ),
        data={
            'channelId': 'smart-home-neighborhood'
        
        },  
        #token=registration_token,  
        topic=topic,
        
    
    )
    patt_message = messaging.Message(
        notification = messaging.Notification(
                title = "Lights and Fans have been automatically turned on",
                body = "The Lights and Fans have been automatically turned as per your normal routine."
                ),
        data={
            'channelId': 'smart-home-pattern'
        
        },  
        #token=registration_token,   
        topic=topic,

    
    )
    cam_message = messaging.Message(
        notification = messaging.Notification(
                title = "Indoor Security camera is turned on",
                body = "The indoor security camera has been triggered as we have detected human presence while you were away"
                ),
        data={
            'channelId': 'smart-home-indoor'
        
        },  
        #token=registration_token,   
        topic=topic,
    
    )
    app.run(host = '0.0.0.0')
