import pytest
from temperature_sensor_wrapper import init_circular_buffer, add_to_buffer, get_temperature

def test_circular_buffer_initialization():
    cb = init_circular_buffer()
    assert cb.head == 0
    assert cb.tail == 0
    assert cb.count == 0

def test_buffer_add():
    cb = init_circular_buffer()
    add_to_buffer(cb, 42)  # Add a value
    assert cb.buffer[0] == 42
    assert cb.head == 1
    assert cb.count == 1

    # Add more than BUFFER_SIZE values to test circular behavior
    for i in range(1, 15):  # Add 14 more values
        add_to_buffer(cb, i)

    # Check circular behavior
    assert cb.count == 10
    assert cb.buffer[0] == 10
    assert cb.head == 5 
    assert cb.tail == 5

def test_read_temperature():
    temp = get_temperature()
    assert 0 <= temp <= 150  # Ensure the temperature is within the expected range