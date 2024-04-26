from evdev import UInput, ecodes as e
import uinput

device = uinput.Device([
    uinput.KEY_X,
    uinput.KEY_Z,
    uinput.KEY_UP,
    uinput.KEY_DOWN,
    uinput.KEY_LEFT,
    uinput.KEY_RIGHT,
    uinput.KEY_UP,
    uinput.KEY_VOLUMEUP,
    uinput.KEY_VOLUMEDOWN
])


with UInput(device) as ui:
    ui.emit_click(uinput.KEY_Z)
    ui.syn()


