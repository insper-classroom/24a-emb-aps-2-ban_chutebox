import serial
import uinput
import pyautogui

ser = serial.Serial('/dev/ttyACM0', 115200)

device = uinput.Device([
    uinput.BTN_LEFT,
    uinput.BTN_RIGHT,
    uinput.BTN_X,
    uinput.BTN_Z,
])

def parse_data(data):
    axis = data[0]
    value = int.from_bytes(data[1:3], byteorder='big', signed=True)
    print(f"Received data: {data}")
    print(f"axis: {axis}, value: {value}")
    return axis, value

def handle_button_press(axis, value):
    if axis == 1:
        if value:  # value is non-zero (usually 1)
            pyautogui.keyDown('x')
        else:
            pyautogui.keyUp('x')
    elif axis == 2:
        if value:
            pyautogui.keyDown('z')
        else:
            pyautogui.keyUp('z')
    elif axis == 3:
        if value:
            pyautogui.keyDown('left')
        else:
            pyautogui.keyUp('left')
    elif axis == 4:
        if value:
            pyautogui.keyDown('right')
        else:
            pyautogui.keyUp('right')

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

