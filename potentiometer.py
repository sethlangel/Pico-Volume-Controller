import board
import analogio
import time
import busio

# Initialize the potentiometers on A0 and A1
potentiometer0 = analogio.AnalogIn(board.A0)
potentiometer1 = analogio.AnalogIn(board.A1)
potentiometer2 = analogio.AnalogIn(board.A2)

# Initialize UART on GP0 (TX) and GP1 (RX)
uart = busio.UART(board.GP0, board.GP1, baudrate=9600)

def map_range(value, in_min, in_max, out_min, out_max):
    return (value - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

def get_smoothed_value(potentiometer, num_samples=5):
    total = 0
    for _ in range(num_samples):
        total += potentiometer.value
        time.sleep(0.01)
    return total // num_samples

threshold = 1
last_volume_0 = map_range(potentiometer0.value, 0, 65535, 0, 100)
last_volume_1 = map_range(potentiometer1.value, 0, 65535, 0, 100)
last_volume_2 = map_range(potentiometer2.value, 0, 65535, 0, 100)

while True:
    current_value_0 = get_smoothed_value(potentiometer0)
    current_volume_0 = map_range(current_value_0, 0, 65535, 0, 100)
    
    current_value_1 = get_smoothed_value(potentiometer1)
    current_volume_1 = map_range(current_value_1, 0, 65535, 0, 100)
    
    current_value_2 = get_smoothed_value(potentiometer2)
    current_volume_2 = map_range(current_value_2, 0, 65535, 0, 100)
    
    if abs(current_volume_0 - last_volume_0) > threshold:
        data = f"0_{current_volume_0}\n"
        print(data)  # Print to console for debugging
        uart.write(data.encode())
    elif abs(current_volume_1 - last_volume_1) > threshold:
        data = f"1_{current_volume_1}\n"
        print(data)  # Print to console for debugging
        uart.write(data.encode())
    elif abs(current_volume_2 - last_volume_2) > threshold:
        data = f"2_{current_volume_2}\n"
        print(data)  # Print to console for debugging
        uart.write(data.encode())
        

    last_volume_0 = current_volume_0
    last_volume_1 = current_volume_1
    last_volume_2 = current_volume_2
        
    time.sleep(0.05)