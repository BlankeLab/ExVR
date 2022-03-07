
import sys
#sys.path.append(r"C:\Python27amd64\Lib")

from utility import*

#import numpy


# class to subclass
class Component(object):

    self_callback = None
    external_callback = None

    def __init__(self, self_callback, external_callback):
        self.self_callback  = self_callback
        self.external_callback = external_callback        
        
    # Get the callback for calling a function of this component in unity
    def this(self):
        return self.self_callback

    # Get the callback for calling a function from any component in Unity
    def other(self):
        return self.external_callback

    # overwrite functions below
    def initialize(self):                
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

    def update_from_current_config(self):
        pass        
        
    def update_parameter_from_gui(self, parameterName):
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
