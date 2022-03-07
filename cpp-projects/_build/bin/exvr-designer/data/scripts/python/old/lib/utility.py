
#import math, os, csv, time, threading, System, sys, clr

import math
import System
from System import Single
from System import Array
from System.Collections.Generic import *

class UnityType:
    Void = 0
    Color   = 1
    Vector2 = 2
    Vector3 = 3
    Vector4 = 4
    Rect    = 5
    Ray     = 6
    Quaternion = 7
    Plane   = 8
    Texture2D = 9
    Array1D = 10
    Array2D = 11
    List1D  = 12
    String  = 13
    Integer  = 14
    Single  = 15
    Boolean  = 16

# python unity types
class Color(object):

    rgba = [0.0,0.0,0.0,0.0]

    def r(self):
        return self.rgba[0]

    def g(self):
        return self.rgba[1]

    def b(self):
        return self.rgba[2]

    def a(self):
        return self.rgba[3]

    def col(self, id):
        return self.rgba[id]

    def __init__(self,r,g,b,a):
        self.rgba = [r,g,b,a]
                
    def to_unity(self):   
        a = Array.CreateInstance(Single, 4)
        a[0] = self.rgba[0]
        a[1] = self.rgba[1]
        a[2] = self.rgba[2]
        a[3] = self.rgba[3]
        return a  
        
class Vector2(object):

    x = 0.0
    y = 0.0

    def __init__(self,x,y):
        self.x = x
        self.y = y
    
    def to_unity(self):
        a = Array.CreateInstance(Single, 2)
        a[0] = self.x
        a[1] = self.y
        return a

class Vector3(object):

    x = 0.0
    y = 0.0
    z = 0.0

    def __init__(self,x,y,z):
        self.x = x
        self.y = y
        self.z = z
        
    def to_unity(self):
        a = Array.CreateInstance(Single, 3)
        a[0] = self.x
        a[1] = self.y
        a[2] = self.z
        return a        

class Vector4(object):

    x = 0.0
    y = 0.0
    z = 0.0
    w = 0.0

    def __init__(self,x,y,z,w):
        self.x = x
        self.y = y
        self.z = z
        self.w = w
        
    def to_unity(self):        
        a = Array.CreateInstance(Single, 4)
        a[0] = self.x
        a[1] = self.y
        a[2] = self.z
        a[3] = self.w
        return a          
        
class Rect(object):

    pos = Vector2(0,0)
    size = Vector2(0,0)

    def __init__(self,pos, size):
        self.pos = pos
        self.size = size

    def to_unity(self):        
        a = Array.CreateInstance(Single, 4)
        a[0] = self.pos.x
        a[1] = self.pos.y
        a[2] = self.size.x
        a[3] = self.size.y
        return a     
        
class Ray(object):

    origin = Vector2(0,0)
    dir = Vector2(0,0)

    def __init__(self,origin, dir):
        self.origin = origin
        self.dir = dir

    def to_unity(self):        
        a = Array.CreateInstance(Single, 4)
        a[0] = self.origin.x
        a[1] = self.origin.y
        a[2] = self.dir.x
        a[3] = self.dir.y
        return a     

class Quaternion(Vector4):
    pass        

class Plane(object):
    
    normal = Vector3(0,0,0)
    point = Vector3(0,0,0)
    
    def __init__(self,normal, point):
        self.normal = normal
        self.point = point
        
    def to_unity(self):        
        a = Array.CreateInstance(Single, 6)
        a[0] = self.normal.x
        a[1] = self.normal.y
        a[2] = self.normal.z
        a[3] = self.point.x
        a[4] = self.point.y
        a[5] = self.point.z
        return a     


def Texture2D_unity(colors_array2D):
    a = Array.CreateInstance(Single, len(colors_array2D)*4, len(colors_array2D[0]))
    indexes = [0, 1, 2, 3]
    for i, row in enumerate(colors_array2D):        
        for j, col in enumerate(row):
            for k, id in enumerate(indexes):
                a[4*i + k, j] = Single(col.col(k))
    return a
    
        
# convert python to unity types functions
def Array1D_unity(data):    
    a = Array.CreateInstance(type(data[0]), len(data))
    for i, row in enumerate(data):
        a[i] = row
    return a

def Array1D_unity(data, type):    
    a = Array.CreateInstance(type, len(data))
    for i, row in enumerate(data):
        a[i] = row
    return a
    
def Array2D_unity(data, type):
    a = Array.CreateInstance(type, len(data), len(data[0]))
    for i, row in enumerate(data):
        for j, col in enumerate(row):
            a[i, j] = col
    return a    

def List1D_unity(data, type):
    l = List[type](len(data))
    for i, row in enumerate(data):
        l.Add(row)
    return l


# Unity - Python types correspondance
# -> float  - single
# -> double - float
# -> double - double
# -> string - str/string
# -> int    - int
def convert_return_values(returnValues):

    idR = returnValues[0]
    if idR == UnityType.Void:
        return

    values = returnValues[1]

    if idR == UnityType.Vector3:
        return Vector3(values[0],values[1],values[2])
    elif idR == UnityType.Vector4:
        return Vector4(values[0],values[1],values[2],values[3])
    elif idR == UnityType.Vector2:
        return Vector2(values[0],values[1])
    elif idR == UnityType.Color:
        return Color(values[0],values[1],values[2],values[3])
    elif idR == UnityType.Rect:
        return Rect(Vector2(values[0],values[1]),Vector2(values[2],values[3]))
    elif idR == UnityType.String:
        return str(values)
    elif idR == UnityType.Boolean:
        return values == 1        
    return values

# callbacks functions
def call_self_unity(this, functionName, argsExport):
    return convert_return_values(this(functionName, List[object](argsExport)))
    
def call_unity(other, routineName, componentName, functionName, argsExport): 
    return convert_return_values(other(routineName, componentName, functionName, List[object](argsExport)))

###  public utility functions for components
# log message
def log(this, message):
    call_self_unity(this, "log", [message])

# error log message
def log_error(this, errorMessage):
    call_self_unity(this, "log_python_error", [errorMessage])

# disable the component, it will be not enabled again before the next routine
def close(this):
    call_self_unity(this, "close", [])
        
# Return the name of the component
def component_name(this):
    return call_self_unity(this, "component_name", [])

def get_init_config_parameter(this, parameterName):
    return call_self_unity(this, "get_init_config_from_python", [parameterName])    
    
def get_current_config_parameter(this, parameterName):
    return call_self_unity(this, "get_current_config_from_python", [parameterName])    
