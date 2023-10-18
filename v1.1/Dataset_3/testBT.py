# import serial

# # Replace 'COM4' with the COM port for your ESP32
# ser = serial.Serial('COM5', baudrate=115200)  # Adjust the baud rate to match your ESP32

# try:
#     print("hi1")
#     for _ in range(1):  # Replace 100 with the desired number of iterations
#         print("hi2")
#         received_data = ser.readline().decode('utf-8').strip()
#         print("hi3")
#         print("Received data from ESP32:", received_data)
#         print("hi4")
# except KeyboardInterrupt:
#     print("Program terminated.")
# finally:
#     ser.close()

print("starting")

from tensorflow.keras.models import load_model
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from keras.callbacks import EarlyStopping

model = load_model("15_point_plastic_classifier_model_2_10_HvL.keras")

import numpy as np  # Add this line to import the NumPy library
import serial
from sklearn.preprocessing import LabelEncoder


# Open the serial port
ser = serial.Serial('COM4', baudrate=9600)  # Adjust the baudrate as needed

# Initialize a variable to store the received data
received_data = ""

# print(label_encoder.classes_)

print("ready")

try:
    while True:
        # Read a line of data from the serial port (assuming data is sent as lines)
        line = ser.readline().decode('utf-8').strip()

        # Split the received data into a list of numbers using a comma as the delimiter
        data_values = [float(val) for val in line.split(',')]

        print(line)
        
        # Normalize the data values if needed
        normalized_data = [(val - 0) / (8410000 - 0) for val in data_values]
        
        # Make predictions on the normalized data
        predictions = model.predict([normalized_data])  # Convert normalized_data to a list
        
        # Format the model so that its easier to read in terminal 
        formatted_predictions = np.around(predictions, 4)

        # Get the top prediction
        top_prediction = np.argmax(predictions)

        # top_plastic_type = label_encoder.inverse_transform([top_prediction])[0]
        # print(top_plastic_type)
        # likelihood = predictions[0][top_prediction] * 100
        

        if predictions.shape[1] == 6:
            plastic_types = ["HDPE", "LDPE", "PET", "PP", "PVC", "Unknown"]
        else:
            plastic_types = ["PE", "PET", "PP", "PVC", "Unknown"]
        
        top_plastic_type = plastic_types[top_prediction]

        likelihood = predictions[0][top_prediction] * 100


        # Display the prediction for manual input
        print(f"Manual Input Prediction:")
        print(f"  Prediction: {top_plastic_type} with {likelihood:.2f}% likelihood")

        received_data = ""

        # Send the top plastic type back over serial
        # ser.write(top_plastic_type.encode('utf-8'))

        likelihood_str = round(likelihood)-1
        data_to_send = f"{top_plastic_type},{likelihood_str}"  # Combine the data fields with a comma delimiter
        ser.write(data_to_send.encode('utf-8'))

except KeyboardInterrupt:
    # Handle Ctrl+C to stop the loop gracefully
    pass

finally:
    # Close the serial port when done
    ser.close()

# Now, you can use the 'received_data' variable for further processing
print("total data:\n", received_data)