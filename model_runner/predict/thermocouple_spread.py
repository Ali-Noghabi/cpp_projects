# -*- coding: utf-8 -*-
"""
Created on Fri Jan  5 19:14:05 2024

@author: Hamideh Jafari

Thermocouple Spread
Input: Turbine Outlet Temperatures (24 Thermocouple)
Ouput: Temp. Deviation, Which Thermocouple, Angle
"""

import numpy as np

def thermocouple_spread(input_data, TETC=525, constant_param=True):
    
    TET = np.array([input_data['TET1'], input_data['TET2'], input_data['TET3'], 
                   input_data['TET4'], input_data['TET5'], input_data['TET6'], 
                   input_data['TET7'], input_data['TET8'], input_data['TET9'], 
                   input_data['TET10'], input_data['TET11'], input_data['TET12'], 
                   input_data['TET13'], input_data['TET14'], input_data['TET15'], 
                   input_data['TET16'], input_data['TET17'], input_data['TET18'], 
                   input_data['TET19'], input_data['TET20'], input_data['TET21'], 
                   input_data['TET22'], input_data['TET23'], input_data['TET24']])
    
    temp_dev = max(abs(TET - TETC))
    
    which_therm = np.where(np.array(abs(TET - TETC)) == max(abs(TET - TETC)))   
    
    if len(which_therm[0]) > 1:
        angle = []
        for i in range(0,len(which_therm[0])):
            angle.append((360/24)*which_therm[0][i])
    else:
        angle = (360/24)*which_therm[0]
    
    overal_therm_health = 100-(len(which_therm[0])/24*100)
    
    if len(which_therm[0]) > 1:
        output1 = {}
        for i in range(0,len(which_therm[0])):
            output1['which_therm_'+str(i)] = which_therm[0][i]
            output1['angle_'+str(i)] = angle[i]
        output2 = {'temp_dev':temp_dev, 'Overal Therm. Health':overal_therm_health}
        output2.update(output1)
        return output2
    else:
        return {'temp_dev':temp_dev,
                'Overal Therm. Health':overal_therm_health,
                'which_therm':which_therm[0], 
                'Therm_angle':angle}
