import uinput
from evdev import UInput, ecodes as e

device = uinput.Device([
    uinput.KEY_X
])

with UInput(device) as ui:
    ui.emit_click(uinput.KEY_X)
    ui.syn()