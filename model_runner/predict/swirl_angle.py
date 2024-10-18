# -*- coding: utf-8 -*-
"""
Created on Fri Jan  5 17:46:50 2024

@author: Hamideh Jafari

Swirl Angle
Input: Comp. Outlet Temp. & Speed
Ouput: Swirl Angle
"""

from keras.models import load_model
import pandas as pd
import numpy as np


def swirl_angle(input_data, constant_param=True):
    
    P3 = input_data['P3']
    Speed = input_data['Speed']
    P0 = input_data['P0']
    T0 = input_data['T0']
    gamma = 1.4
    R = 287
    input_data = pd.Series()    
    input_data['Corr_Speed'] = Speed/(((T0+273)*R*gamma)**(0.5))
    input_data['Pr_C'] = P3/P0
    input_data['constant coefficient'] = 25.1146
    
    swirl_angle_model = load_model('ML_swirl_angle.h5')
    angle = swirl_angle_model.predict(np.reshape(np.array(input_data), (1,3)))*10
    
    return {'swirl_angle':list(angle[0])[0]}
