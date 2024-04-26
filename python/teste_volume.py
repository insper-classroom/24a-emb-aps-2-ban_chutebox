import subprocess
import time

def set_volume(level):
    # Check if the input level is within the valid range
    if not 0 <= level <= 9:
        raise ValueError("Volume level must be between 0 and 9")
    
    # Calculate the percentage volume from the level (0 to 9)
    volume_percent = level * 10  # This will convert 0-9 to 0%-90%
    
    # Set the volume using the pactl command
    subprocess.run(['pactl', 'set-sink-volume', '@DEFAULT_SINK@', f'{volume_percent}%'], check=True)

# Example usage
set_volume(5)  # Sets the volume to approximately 50%
time.sleep(5)
set_volume(9)
time.sleep(5)
set_volume(0)
