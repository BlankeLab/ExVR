
#from utility import * # check utility.py for functions documentation
#from component import *

#A = np.arange(10)
#print('input list = ',A)

import ex
import sys
import numpy as np

class Component(object):

    def initialize(self):        
        ex.log("From python: initialize ")    
        ex.log(sys.path[0])
        return True

    def start_experiment(self):
        ex.log("From python: start_experiment ")
        pass

    def stop_experiment(self):
        ex.log("From python: stop_experiment ")
        pass

    def start_routine(self):
        ex.log("From python: start_routine ")
        pass

    def stop_routine(self):
        ex.log("From python: stop_routine ")
        pass

    def update(self):       
        pass

    def update_parameter_from_gui(self):
        pass        
        
    def stop(self):
        pass

    def play(self):
        pass

    def pause(self):
        pass

    def set_visibility(self, visible):
        #ex.log("From python: set_visibility " + str(visible))
        pass

    def set_update_state(self, doUpdate):
        #ex.log("From python: set_update_state " + str(doUpdate))
        pass

    def clean(self):
        pass


class DefaultComponent(Component):
    
    def update(self):       
        pass


        

