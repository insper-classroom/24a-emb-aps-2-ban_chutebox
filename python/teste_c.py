import time
from pynput.keyboard import Controller, Key

keyboard = Controller()

for i in range(0, 100):
    keyboard.press(Key.media_volume_up)
    time.sleep(0.5)

print('acabei')