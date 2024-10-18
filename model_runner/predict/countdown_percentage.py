# -*- coding: utf-8 -*-
"""
Created on Fri Jan  5 16:48:28 2024

@author: Hamideh Jafari

Countdown (Filter or Compressor Washing)
Input: filter changing time & maximum life of filter
Ouput: life of filter
"""

import dateutil.parser as dtp
from datetime import datetime

def Countdown_Percentage(changing_time, max_day=100, constant_param=True):
    
    changing_time = dtp.parse(str(changing_time)).timestamp()
    now_time = datetime.today().timestamp()
    passed_time = (datetime.fromtimestamp(now_time - changing_time).day)-1
    remaining_time = max_day - passed_time
    precent_remaining_time = (remaining_time/max_day)*100
    
    return {'precent_remaining_time':precent_remaining_time}
