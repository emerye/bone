from machine import Pin, PWM
import sys
import time


pwm_pin = Pin(21, Pin.OUT)
pwm = PWM(pwm_pin, freq=10000, duty_u16=32767)

print("--- Sparrow Detect Interactive PWM Control --- Version 0.02 GPI021")
print("Enter a duty cycle percentage from 0 to 100. A decimal number is allowed. Default is 50.0")
print("Type 'exit' to quit.\n")


while True:
    try:
        user_input = input("Enter duty cycle % (0-100): ").strip().lower()
        
        if user_input == 'exit':
            print("Exiting and deinitializing PWM.")
            pwm.deinit()
            break
            
        percentage = float(user_input)
        if not (0 <= percentage <= 100):
            print("Error: Please enter a value between 0 and 100.")
            continue
            
        duty_value = int((percentage / 100) * 65535)
        
        pwm.duty_u16(duty_value)
        print(f"Set duty cycle to {percentage}% (Raw 16-bit value: {duty_value})")
        
    except ValueError:
        print("Invalid input. Please enter a valid number or 'exit'.")
    except KeyboardInterrupt:
        print("\nProgram interrupted. Cleaning up.")
        pwm.deinit()
        break



50