# -*- coding: utf-8 -*-
"""
Created on Wed Feb 19 01:07:38 2020

@author: User
"""


def find_Pred():
    from firebase import firebase
    import pandas as pd
    import numpy as np
    from statsmodels.tsa.api import  SimpleExpSmoothing
    import datetime as dt



    firebase = firebase.FirebaseApplication("https://smarthomefyp-f9244.firebaseio.com/")
    res = firebase.get('/patterntimestamps',None)
    res = pd.DataFrame(res.items())

    res= res[1]
    fit = SimpleExpSmoothing(np.asarray(res)).fit(smoothing_level = 0.6, optimized = False)
    fit2 = fit.forecast(1)
    fit2 = fit2[0]
    fin = str(dt.timedelta(hours=fit2)).rsplit(':', 1)[0]
    firebase.put("/","prediction",fin)
    return fin





