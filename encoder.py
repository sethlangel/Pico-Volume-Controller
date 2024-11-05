import board
import time
import digitalio

class Encoder:
    def __init__(self, stepPin, dirPin, button, encoder_id):
        self.stepPin = stepPin
        self.dirPin = dirPin
        self.button = button
        self.id = encoder_id
        self.prevVal = stepPin.value
        self.toggle_value = False
        self.pressed = False
    
    def update(self):
        if self.button.value == 1 and not self.pressed:
            self.toggle_value = not self.toggle_value
            self.pressed = True
            print(f"{{{self.id}: {self.toggle_value}}}")
        
        elif self.button.value == 0:
            self.pressed = False
        
        if self.prevVal != self.stepPin.value:
            if self.stepPin.value == False:
                if self.dirPin.value == False:
                    print(f"{{{self.id}: left}}")
                else:
                    print(f"{{{self.id}: right}}")
            self.preVal = self.stepPin.value
             
            time.sleep(.1)
            
def setup_pin(pin_number, pull=digitalio.Pull.UP):
    pin = digitalio.DigitalInOut(pin_number)
    pin.direction = digitalio.Direction.INPUT
    pin.pull = pull
    return pin

button_pins = [board.GP16, board.GP17, board.GP18]
dir_pins = [board.GP14, board.GP10, board.GP12]
step_pins = [board.GP15, board.GP11, board.GP13]

encoders = []
for i in range(3):
    button = digitalio.DigitalInOut(button_pins[i])
    button.switch_to_input(pull=digitalio.Pull.DOWN)
    dir_pin = setup_pin(dir_pins[i])
    step_pin = setup_pin(step_pins[i])
    encoder = Encoder(step_pin, dir_pin, button, encoder_id=i)
    encoders.append(encoder)


while True:
    for encoder in encoders:
        encoder.update()
    
    time.sleep(0.01)
