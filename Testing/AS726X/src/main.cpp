#include <Arduino.h>
#include "AS726X.h"

AS726X sensor; // Creates the sensor object
byte GAIN = 0;
byte MEASUREMENT_MODE = 0;

// Button stuff
const int buttonPin = 12; // The pin number where the button is connected
int buttonState = 0;      // Variable to store the button state

float R = 0;
float S = 0;
float T = 0;
float U = 0;
float V = 0;
float W = 0;

void sensorStuff()
{

  for (int i = 0; i < 3; i++)
  {
    sensor.takeMeasurements();

    R += sensor.getCalibratedR();
    S += sensor.getCalibratedS();
    T += sensor.getCalibratedT();
    U += sensor.getCalibratedU();
    V += sensor.getCalibratedV();
    W += sensor.getCalibratedW();
  }

  R = R / 3;
  S = S / 3;
  T = T / 3;
  U = U / 3;
  V = V / 3;
  W = W / 3;

  Serial.print(R);
  Serial.print(",");
  Serial.print(S);
  Serial.print(",");
  Serial.print(T);
  Serial.print(",");
  Serial.print(U);
  Serial.print(",");
  Serial.print(V);
  Serial.print(",");
  Serial.print(W);

  Serial.println();
}

void buttonStuff()
{
  buttonState = digitalRead(buttonPin); // Read the state of the button (LOW when pressed, HIGH when not pressed)

  if (buttonState == LOW)
  {
    sensorStuff();
  }

  delay(100); // Add a small delay to avoid rapid bouncing readings
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  sensor.begin(Wire, GAIN, MEASUREMENT_MODE); // Initializes the sensor with non default values

  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as INPUT_PULLUP (internal pull-up resistor)
}

void loop()
{
  // buttonStuff();
  // sensorStuff();
  sensor.takeMeasurementsWithBulb();
}
