import ctypes
from ctypes import POINTER, Structure, c_int

# Load the shared library
lib = ctypes.CDLL('./temperature_sensor.so')

# Define the CircularBuffer structure
class CircularBuffer(Structure):
    _fields_ = [
        ("buffer", c_int * 10),
        ("head", c_int),
        ("tail", c_int),
        ("count", c_int),
    ]

# Define prototypes for the C functions
lib.buffer_add.argtypes = [POINTER(CircularBuffer), c_int]
lib.read_temperature.restype = c_int

# Wrapper functions
def init_circular_buffer():
    cb = CircularBuffer()
    cb.head = 0
    cb.tail = 0
    cb.count = 0
    return cb

def add_to_buffer(cb, value):
    lib.buffer_add(ctypes.byref(cb), value)

def get_temperature():
    return lib.read_temperature()