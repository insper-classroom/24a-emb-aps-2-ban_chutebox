import time
from pynput.keyboard import Controller, Key

keyboard = Controller()

while True:
    time.sleep(3)
    keyboard.press('x')
    time.sleep(1)
    keyboard.release('x')
    time.sleep(1)
    keyboard.press('z')
    time.sleep(1)
    keyboard.release('z')
    time.sleep(1)
    keyboard.press(Key.left)
    time.sleep(1)
    keyboard.release(Key.left)
    time.sleep(1)
    keyboard.press(Key.right)
    time.sleep(1)
    keyboard.release(Key.right)
    time.sleep(1)
    keyboard.press(Key.down)
    time.sleep(1)
    keyboard.release(Key.down)
