# -*- coding: utf-8 -*-
"""
Created on Thu Feb 27 00:50:08 2020

@author: User
"""
def notif(message):
    from firebase_admin import messaging
    messaging.send(message)
    return "Done :)"
