import serial
import uinput
from pynput.keyboard import Controller, Key
from evdev import UInput, ecodes as e
# from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
# from comtypes import CLSCTX_ALL
# from ctypes import cast, POINTER

button_state_1 = 1
button_state_2  = 1
button_state_3  = 1
button_state_4  = 1
button_state_5 = 1
button_state_6 = 1

keyboard = Controller()
# ser = serial.Serial('/dev/ttyACM0', 115200)
ser = serial.Serial('/dev/rfcomm0', 9600) 

def parse_data(data):
    axis = data[0]
    value = int.from_bytes(data[1:3], byteorder='big', signed=True)
    print(f"Received data: {data}")
    print(f"axis: {axis}, value: {value}")
    return axis, value

# def set_volume(level):
#     if not 0 <= level <= 9:
#         raise ValueError("Volume level must be between 0 and 9")
    
#     devices = AudioUtilities.GetSpeakers()
#     interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
#     volume = cast(interface, POINTER(IAudioEndpointVolume))

#     volume_range = volume.GetVolumeRange()
#     min_vol, max_vol = volume_range[0], volume_range[1]
#     scaled_vol = min_vol + (level / 9) * (max_vol - min_vol)
    
#     volume.SetMasterVolumeLevel(scaled_vol, None)

device = uinput.Device([
    uinput.KEY_Xc,
    uinput.KEY_Zc,
    uinput.KEY_UPc,
    uinput.KEY_DOWNc,
    uinput.KEY_LEFTc,
    uinput.KEY_RIGHTc,
    uinput.KEY_UPc,
    uinput.KEY_VOLUMEUPc,
    uinput.KEY_VOLUMEDOWNc
])

def handle_button_press(axis, value): 

    global button_state_1
    global button_state_2
    global button_state_3
    global button_state_4
    global button_state_5
    global button_state_6
    
    if axis == 1: 
        with UInput(device) as ui:
            ui.emit_click(uinput.KEY_X)
            ui.syn()
    elif axis == 2: 
        with UInput(device) as ui:
            ui.emit_click(uinput.KEY_Z)
            ui.syn()
    elif axis == 3: 
        with UInput(device) as ui:
            ui.emit_click(uinput.KEY_Z)
            ui.syn()
    elif axis == 4: 
        with UInput(device) as ui:
            ui.emit_click(uinput.KEY_Z)
            ui.syn()
    elif axis == 5: 
        with UInput(device) as ui:
            ui.emit_click(uinput.KEY_Z)
            ui.syn()
    elif axis == 6: 
        with UInput(device) as ui:
            ui.emit_click(uinput.KEY_Z)
            ui.syn()
    elif axis == 7: 
        with UInput(device) as ui:
            ui.emit_click(uinput.KEY_Z)
            ui.syn()
    elif axis == 8: 
        with UInput(device) as ui:
            ui.emit_click(uinput.KEY_Z)
            ui.syn()
    

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