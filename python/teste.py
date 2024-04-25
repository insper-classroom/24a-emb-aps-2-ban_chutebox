import serial
import uinput
from pynput.keyboard import Controller, Key

button_state_1 = 1
button_state_2  = 1
button_state_3  = 1
button_state_4  = 1
button_state_5 = 1
button_state_6 = 1

keyboard = Controller()
ser = serial.Serial('/dev/ttyACM0', 115200)
#ser = serial.Serial('/dev/rfcomm0', 9600) 

def parse_data(data):
    axis = data[0]
    value = int.from_bytes(data[1:3], byteorder='big', signed=True)
    print(f"Received data: {data}")
    print(f"axis: {axis}, value: {value}")
    return axis, value

def handle_button_press(axis, value): 

    global button_state_1
    global button_state_2
    global button_state_3
    global button_state_4
    global button_state_5
    global button_state_6
    
    if axis == 1: 
        if value and button_state_1 != 1:
            keyboard.press('x')
            button_state_1 = 1
        elif not value and button_state_1 != 0:
            keyboard.release('x')
            button_state_1 = 0
    elif axis == 2: 
        if value and button_state_2 != 1:
            keyboard.press('z')
            button_state_2 = 1
        elif not value and button_state_2 != 0:
            keyboard.release('z')
            button_state_2 = 0
    elif axis == 3: 
        if value and button_state_3 != 1:
            keyboard.press(Key.left)
            button_state_3 = 1
        elif not value and button_state_3 != 0:
            keyboard.release(Key.left)
            button_state_3 = 0
    elif axis == 4: 
        if value and button_state_4 != 1:
            keyboard.press(Key.right)
            button_state_4 = 1
        elif not value and button_state_4 != 0:
            keyboard.release(Key.right)
            button_state_4 = 0
    elif axis == 5: 
        if value and button_state_5 != 1:
            keyboard.press(Key.down)
            button_state_5 = 1
        elif not value and button_state_5 != 0:
            keyboard.release(Key.down)
            button_state_5 = 0
    elif axis == 6: 
        if value and button_state_6 != 1:
            keyboard.press(Key.up)
            button_state_6 = 1
        elif not value and button_state_6 != 0:
            keyboard.release(Key.up)
            button_state_6 = 0

try:
    while True:
        print('Waiting for sync package...')
        while True:
            data = ser.read(1)
            if data == b'\xff':
                break

        data = ser.read(3)
        axis, value = parse_data(data)
        handle_button_press(axis, value)

except KeyboardInterrupt:
    print("Program terminated by user.")
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    ser.close()