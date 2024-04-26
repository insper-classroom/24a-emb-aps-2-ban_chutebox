import serial
import uinput
from pynput.keyboard import Controller, Key
import subprocess
import time

volume = None

keyboard = Controller()
# ser = serial.Serial('/dev/ttyACM0', 115200)
ser = serial.Serial('/dev/rfcomm0', 9600) 

def parse_data(data):
    axis = data[0]
    value = int.from_bytes(data[1:3], byteorder='big', signed=True)
    print(f"Received data: {data}")
    print(f"axis: {axis}, value: {value}")
    return axis, value

def set_volume(level):
    if not 0 <= level <= 9:
        raise ValueError("Volume level must be between 0 and 9")
    volume_percent = level * 10
    subprocess.run(['pactl', 'set-sink-volume', '@DEFAULT_SINK@', f'{volume_percent}%'], check=True)


def handle_button_press(axis, value): 

    global volume

    if axis == 1: 
        keyboard.press(Key.esc)
        time.sleep(0.05)
        keyboard.release(Key.esc)
    elif axis == 2: 
        keyboard.press('z')
        time.sleep(0.05)
        keyboard.release('z')
    elif axis == 3:
        keyboard.press(Key.left)
        time.sleep(0.05)
        keyboard.release(Key.left)
    elif axis == 4:
        keyboard.press(Key.right)
        time.sleep(0.05)
        keyboard.release(Key.right)
    elif axis == 5:
        keyboard.press(Key.down)
        time.sleep(0.15)
        keyboard.release(Key.down)
    # elif axis == 6:
    #     keyboard.press(Key.esc)
    #     time.sleep(0.05)
    #     keyboard.release(Key.esc)
    elif axis == 7:
        set_volume(value)
        print(f'Volume: {value}')


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