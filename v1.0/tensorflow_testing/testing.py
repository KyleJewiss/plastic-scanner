import pandas as pd
import tensorflow as tf
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split

# Load the data from the concatenated CSV file
data = pd.read_csv("tensorflow_testing/allTogether.csv")

# Extract features (IR wavelength values) and target (plastic type labels)
features = data.drop("Plastic_Type", axis=1)
labels = data["Plastic_Type"]

# Handle negative values (e.g., by normalization)
features = (features - features.min()) / (features.max() - features.min())

# Use Label Encoding to convert target labels into numerical values
label_encoder = LabelEncoder()
encoded_labels = label_encoder.fit_transform(labels)

# Encode the target labels into one-hot encoded format
encoded_labels = tf.keras.utils.to_categorical(encoded_labels, num_classes=4)  # 4 classes (3 plastics + "other")

# Split the data into training and testing sets (80% training, 20% testing)
X_train, X_test, y_train, y_test = train_test_split(features, encoded_labels, test_size=0.2, random_state=42)

# Build the model
model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(8,)),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(4, activation='softmax')  # 4 output units for 3 plastic types and "other"
])

# Compile the model with categorical_crossentropy loss
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

# Early stopping callback
early_stopping = tf.keras.callbacks.EarlyStopping(patience=10, restore_best_weights=True)

# Train the model
model.fit(X_train, y_train, epochs=200, batch_size=32, validation_split=0.1, callbacks=[early_stopping])

# Evaluate the model on the testing set
loss, accuracy = model.evaluate(X_test, y_test)
print(f"Test Accuracy: {accuracy:.4f}")


# Example: Predicting the plastic type for a new set of IR reflectance values
new_data = pd.DataFrame({
    "IR_Wavelength_1": [158],
    "IR_Wavelength_2": [1775.67],
    "IR_Wavelength_3": [5830],
    "IR_Wavelength_4": [10398.33],
    "IR_Wavelength_5": [16155.33],
    "IR_Wavelength_6": [13835],
    "IR_Wavelength_7": [5455.33],
    "IR_Wavelength_8": [2596.33]
})

# Preprocess the new data (e.g., normalization) and predict the plastic type probabilities
new_data_normalized = (new_data - features.min()) / (features.max() - features.min())
predictions = model.predict(new_data_normalized)

# Decode the predictions to get the plastic type probabilities
plastic_type_probabilities = predictions[0][:3]  # First three elements are the probabilities for HDPE, LDPE, and PE
other_probability = predictions[0][3]  # Fourth element is the probability for "other"

print("Plastic Type Probabilities:")
print("HDPE:", plastic_type_probabilities[0])
print("LDPE:", plastic_type_probabilities[1])
print("PP:", plastic_type_probabilities[2])
print("Other:", other_probability)
