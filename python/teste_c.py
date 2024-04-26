import time
from pynput.keyboard import Controller, Key

keyboard = Controller()

time.sleep(5)
keyboard.press(Key.media_volume_up)
time.sleep(1)
keyboard.press(Key.media_volume_up)
time.sleep(1)
keyboard.press(Key.media_volume_up)
time.sleep(1)
keyboard.press(Key.media_volume_up)
time.sleep(1)
keyboard.press(Key.media_volume_up)
print('acabei')