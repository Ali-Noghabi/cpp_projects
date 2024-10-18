# -*- coding: utf-8 -*-
"""
Created on Fri Jan  5 20:15:45 2024

@author: Hamideh Jafari

Compressor Isentropic Efficiency
Input: Compr. inlet and Outlet Temperatures and Pressure
Ouput: compressor isentropic efficiency
"""

import numpy as np

def comp_is_eff(input_data, constant_param=True):
    
    P2 = input_data['P0']
    T2 = input_data['T0']+273.15
    P3 = input_data['P3']
    T3 = np.nanmean([input_data['T31'],input_data['T32']])+273.15
    
    k = 1.4
    is_eff = (T2/(T3-T2))*((P3/P2)**((k-1)/k)-1)
    
    return {'Comp_IS_Eff':is_eff}
