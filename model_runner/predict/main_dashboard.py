# -*- coding: utf-8 -*-
"""
Created on Fri Jan  5 16:44:57 2024

@author: hamideh
Main function for calculation all output data
"""

import countdown_percentage as cdp
import thermocouple_spread as ts
import kks_dictionary as kks_dic
import power_prediction as pp
import corrected_power as cp
import comp_is_eff as cie
import swirl_angle as sa
import numpy as np

from datetime import datetime, timedelta

#Constant parameters
con_par = {
    'filter_max_day' : 90,
    'comp_washing_max_day' : 60,
    'changing_time_filter' : datetime.today()-timedelta(days = 28),
    'comp_washing_time' : datetime.today()-timedelta(days = 50)
    }

def main_dashboard(input_data, constant_param=True):
    
    input_data.update(con_par)

    #rename keys (KKS to our defination)
    input_data = {kks_dic.kks_dic.get(k, k): v for k, v in input_data.items()}
    
    #Replacing missing sensor
    try:
        if 'T0' not in input_data.keys():
            input_data.update({'T0':input_data['T2']})
    except:
        print('There is not T0/T2')
    
    try:
        corr_power = cp.corrected_power(input_data)
    except:
        corr_power = -1  
        print("Please check correction power input data")
        
    try:
        filter_countdown = cdp.Countdown_Percentage(input_data['changing_time_filter'], input_data['filter_max_day'])
        filter_countdown['precent_remaining_time_filter'] = filter_countdown['precent_remaining_time']
        del filter_countdown['precent_remaining_time']
    except:
        filter_countdown['precent_remaining_time_filter'] = np.nan 
        print("Please check filter input data")
        
    try:
        swirl_ang = sa.swirl_angle(input_data)
    except:
        swirl_ang = -1
        print("Please check swirl angle input data")
        
    try:
        thermo_spread = ts.thermocouple_spread(input_data)
    except:
        thermo_spread = -1
        print("Please check thermocouple spread input data")
        
    try:
        comp_IS_eff = cie.comp_is_eff(input_data)
    except:
        comp_IS_eff = -1
        print("Please check eff. input data")
        
    try:
        comp_washing_countdown = cdp.Countdown_Percentage(input_data['comp_washing_time'], input_data['comp_washing_max_day'])
        comp_washing_countdown['precent_remaining_time_comp_washing'] = comp_washing_countdown['precent_remaining_time']
        del comp_washing_countdown['precent_remaining_time']
    except:
        comp_washing_countdown['precent_remaining_time_comp_washing'] = np.nan
        print("Please check compressor washing input data")
        
    try:
        power_prediction = pp.power_predicton(input_data)
    except:
        power_prediction = -1
        print("Please check power prediction input data")
    
    try:
        timestamp = {"timestamp" : datetime.fromtimestamp(input_data['timestamp']).timestamp()}
    except:
        timestamp = -1
        print("ther is not time")
        
    TIT = {"TIT" : 1124}
    mode = {
        'Cycle_mode' : 1, #"Simple"=1, #Combined=2
        'Fuel_mode' : 1, #"Gas"=1, #Oil=2
        'Load_mode' : 1, #"Base_load"=1, #Part_load=2
        'Shutdown_mode' : 1, #"Shutdown"=1, #Trip=2
        'Start_mode' : 1, #"Start_successful"=1, #Start_unsuccessful=2
        'Filter_changing_time' : con_par['changing_time_filter'].timestamp(),
        'Com_washing_time' : con_par['comp_washing_time'].timestamp()
        }
    
    try:
        output_data = {
            **timestamp,
            **corr_power,
            **filter_countdown,
            **swirl_ang,
            **thermo_spread,
            **comp_IS_eff,
            **comp_washing_countdown,
            **power_prediction,
            **TIT
            }
    except:
        output_data = {
            "timestamp" : timestamp,
            "corr_power" : corr_power,
            **filter_countdown,
            "swirl_ang" : swirl_ang,
            "thermo_spread" : thermo_spread,
            "comp_IS_eff" : comp_IS_eff,
            **comp_washing_countdown,
            "power_prediction" : power_prediction,
            **TIT
            }
        
    output_data.update(mode)
    
    return output_data
