# -*- coding: utf-8 -*-
"""
Created on Fri Jan  5 20:42:23 2024

@author: Hamideh Jafari

Power Prediction
Input: Ambient Condition (Pressure, Temperature, Humidity) $
       Turbine Operation Mode (Cycle, IGV, Fuel)
Ouput: Future Power
"""

from keras.models import load_model
import pandas as pd
import numpy as np

def power_predicton(input_data, constant_param=True):
    
    P0 = input_data['P0']
    T0 = input_data['T0']
    RH = input_data['RH']
    Cycle = input_data['Cycle_mode']
    Fuel = input_data['Fuel_mode']
    
    in_data = pd.Series()    
    in_data['P0'] = P0
    in_data['T0'] = T0
    in_data['RH'] = RH
    
    # if (Cycle.upper() == 'SIMPLE' and Fuel.upper() == 'GAS'):
    #     power_prediction_model = load_model('ML_power_prediction.h5')
    #     power = power_prediction_model.predict(np.reshape(np.array(in_data), (1,3)))*35
    # elif (Cycle.upper() == 'SIMPLE' and Fuel.upper() == 'OIL'):
    #     power_prediction_model = load_model('ML_power_prediction.h5')
    #     power = power_prediction_model.predict(np.reshape(np.array(in_data), (1,3)))*34
    # elif (Cycle.upper() == 'COMBINED' and Fuel.upper() == 'GAS'):
    #     power_prediction_model = load_model('ML_power_prediction.h5')
    #     power = power_prediction_model.predict(np.reshape(np.array(in_data), (1,3)))*34
    # elif (Cycle.upper() == 'COMBINED' and Fuel.upper() == 'OIL'):
    #     power_prediction_model = load_model('ML_power_prediction.h5')
    #     power = power_prediction_model.predict(np.reshape(np.array(in_data), (1,3)))*33
    if (Cycle == 1 and Fuel == 1):
        power_prediction_model = load_model('ML_power_prediction.h5')
        power = power_prediction_model.predict(np.reshape(np.array(in_data), (1,3)))*35
    elif (Cycle == 1 and Fuel == 2):
        power_prediction_model = load_model('ML_power_prediction.h5')
        power = power_prediction_model.predict(np.reshape(np.array(in_data), (1,3)))*34
    elif (Cycle == 2 and Fuel == 1):
        power_prediction_model = load_model('ML_power_prediction.h5')
        power = power_prediction_model.predict(np.reshape(np.array(in_data), (1,3)))*34
    elif (Cycle == 2 and Fuel == 2):
        power_prediction_model = load_model('ML_power_prediction.h5')
        power = power_prediction_model.predict(np.reshape(np.array(in_data), (1,3)))*33

    return {'predicted_power': list(power[0])[0]}
    
