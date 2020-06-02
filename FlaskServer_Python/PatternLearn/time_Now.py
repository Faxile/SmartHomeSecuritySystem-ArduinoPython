# -*- coding: utf-8 -*-
"""
Created on Wed Feb 26 23:48:48 2020

@author: User
"""

def getNow():
    import datetime
    currentDT = datetime.datetime.now()
    return currentDT.strftime("%H:%M")


