print("starting")

from tensorflow.keras.models import load_model
import pandas as pd
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from keras.callbacks import EarlyStopping
import serial

# Load the model
model = load_model("15_point_showcase_model.keras")

# Open the serial port
ser = serial.Serial('COM5', baudrate=9600)  # Adjust the baudrate as needed

# Initialize a variable to store the received data
received_data = ""

# Get labels from csv file
data = pd.read_csv("15_point_showcase.csv")
X = data.drop(columns=["Material_Type"])
y = data["Material_Type"]
label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)

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

        top_plastic_type = label_encoder.inverse_transform([top_prediction])[0]

        likelihood = predictions[0][top_prediction] * 100

        # Display the prediction for manual input
        print(f"Manual Input Prediction:")
        print(f"  Prediction: {top_plastic_type} with {likelihood:.2f}% likelihood")

        received_data = ""

        # Send the top plastic type back over serial
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