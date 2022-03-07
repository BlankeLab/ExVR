
from utility import * # check utility.py for functions documentation
from component import *

import numpy as np

class DefaultComponent(Component):
        
    def initialize(self):
        #print(sys.path)
        log_error(self.this(), sys.path[0])
        log_error(self.this(), sys.path[1])
        log_error(self.this(), sys.path[2])
        #log_error(self.this(), "update_parameter_from_gui " + parameterName)
        return True
        
    def start_experiment(self):
        pass
        
    def start_routine(self):
        pass        
        
    def set_visibility(self, visible):
        pass   

    def set_update_state(self, doUpdate):
        pass   
        
    def pre_update(self):
        pass

    def update(self):
        pass
        
    def post_update(self):
        pass        

    def stop_experiment(self):
        pass

    def update_from_current_config(self):
        pass        
        
    def update_parameter_from_gui(self, parameterName):
        # value = get_current_config_parameter(self.this(), parameterName)        
        #value = get_current_config_parameter(self.this(), parameterName)
        #log_error(self.this(), "value  " + str(value))
        pass        
        
    def stop_routine(self):
        pass        
        
    def stop_experiment(self):
        pass                

    def clean(self):
        pass   
        
    def play(self):
        pass   

    def pause(self):
        pass       
