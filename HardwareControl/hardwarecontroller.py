
try:
    import RPi.GPIO as GPIO
except RuntimeError:
    print("Error importing RPi.GPIO! Try using 'sudo' to run your script")


class Brakes(object):
    """
    Use RPi GPIO pins to control brakes
    """

    def __init__(self):
        self.pin = 12  # Using pin 12
        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(12, GPIO.OUT)

    def engage(self):
        GPIO.output(self.pin, 1)

    def disengage(self):
        GPIO.output(self.pin, 0)


class MotorController(object):
    """
    TODO: Use CAN protocol to manipulate the motor
    """

    def __init__(self):
        pass

    def accelerate(self):
        pass

    def idle(self):
        pass
