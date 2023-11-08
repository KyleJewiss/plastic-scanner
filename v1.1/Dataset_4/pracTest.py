print("starting")

import numpy as np  # Add this line to import the NumPy library
import serial
import csv  # Import the csv module
from sklearn.preprocessing import LabelEncoder
from tensorflow.keras.models import load_model
import pandas as pd
import tensorflow as tf
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from keras.callbacks import EarlyStopping


model = load_model("15_point_showcase_model.keras")

data = pd.read_csv("15_point_showcase.csv")

# Separate features and labels
X = data.drop(columns=["Material_Type"])
y = data["Material_Type"]

# Encode labels
label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)


# Open the serial port
ser = serial.Serial('COM3', baudrate=115200)  # Adjust the baudrate as needed

# Initialize a variable to store the received data
received_data = ""

# Create and open a CSV file for writing
csv_file = open('likelihood.csv', 'w', newline='')
csv_writer = csv.writer(csv_file)

print("ready")

try:
    while True:
        # Read a line of data from the serial port (assuming data is sent as lines)
        line = ser.readline().decode('utf-8').strip()

        # Split the received data into a list of numbers using a comma as the delimiter
        data_values = [float(val) for val in line.split(',')]

        # Normalize the data values if needed
        normalized_data = [(val - 0) / (8410000 - 0) for val in data_values]

        # print(data_values)

        # Make predictions on the normalized data
        predictions = model.predict([normalized_data])  # Convert normalized_data to a list

        # Format the model so that its easier to read in terminal 
        formatted_predictions = np.around(predictions, 4)

        # Get the top prediction
        top_prediction = np.argmax(predictions)

        top_plastic_type = label_encoder.inverse_transform([top_prediction])[0]
        # print(top_plastic_type)
        likelihood = predictions[0][top_prediction] * 100

        # Display the prediction for manual input
        print(f"  Prediction: {top_plastic_type} with {likelihood:.2f}% likelihood")

        likelihood_str = round(likelihood)-1

        # Write the data values to the CSV file as a new row
        csv_writer.writerow([top_plastic_type,likelihood_str])

        received_data = ""

        # Send the top plastic type back over serial
        # ser.write(top_plastic_type.encode('utf-8')) 
        # ser.write(str(likelihood_str).encode('utf-8'))
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