import serial

# Replace 'COM4' with the COM port for your ESP32
ser = serial.Serial('COM5', baudrate=115200)  # Adjust the baud rate to match your ESP32

try:
    print("hi1")
    for _ in range(1):  # Replace 100 with the desired number of iterations
        print("hi2")
        received_data = ser.readline().decode('utf-8').strip()
        print("hi3")
        print("Received data from ESP32:", received_data)
        print("hi4")
except KeyboardInterrupt:
    print("Program terminated.")
finally:
    ser.close()
