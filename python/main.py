import serial
import pyautogui
#from pynput.keyboard import Key, Controller

#keyboard = Controller()
ser = serial.Serial('/dev/ttyACM0', 115200)

def handle_button_press(data):
    if len(data) == 3 and data[0] == 0xFF and data[2] == 0xFE:
        button = data[1]
        print(f'{button}')
        if button == 11:
            pyautogui.keyDown('A')
            #pyautogui.press('A')AAAAAAAAA
            #keyboard.press('A')
            print('button pressed')
        elif button == 10:
            #keyboard.release('A')
            pyautogui.keyUp('A')
            #pyautogui.release('A')
            print('button released')
                                                                                     
try:
    while True:
        data = ser.read(3)  # Read 4 bytes package
        handle_button_press(data)
        

except KeyboardInterrupt:
    print("Program terminated by user.")
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    ser.close()
