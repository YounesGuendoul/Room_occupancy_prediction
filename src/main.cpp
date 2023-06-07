#include <Arduino.h>

#include <dht11.h>
#include <Wire.h>

dht11 DHT11;
const int gasPin = A0;
const int lightPin = A1;

void setup()
{
  Serial.begin(9600);
  initializeSensors();
  createCSVFile();
}

void loop()
{
  delay(1000); 

  int temperature = readTemperature();
  int humidity = readHumidity();
  int CO2 = readGasLevel();
  int light = readLightIntensity();
  float HumidityRatio = calculateHumidityRatio(temperature, humidity);

  appendDataToCSV(temperature, humidity ,CO2, light,HumidityRatio);
}

void initializeSensors()
{
  Wire.begin();
}

int readTemperature()
{
  int chk = DHT11.read(2); 
  return DHT11.temperature;
}

int readHumidity()
{
  int chk = DHT11.read(2);
  return DHT11.humidity;
}

float calculateHumidityRatio(int temperature, int humidity)
{
  float saturationVaporPressure = 6.11 * pow(10, (7.5 * temperature) / (temperature + 237.3));
  float actualVaporPressure = saturationVaporPressure * (humidity / 100.0);
  float humidityRatio = 0.622 * (actualVaporPressure / (101.325 - actualVaporPressure));
  return humidityRatio;
}

int readGasLevel()
{
  int gasValue = analogRead(gasPin);
  int gasPercentage = map(gasValue, 0, 1023, 0, 100);
  return gasPercentage;
}

int readLightIntensity()
{
  int lightValue = analogRead(lightPin);
  int lightPercentage = map(lightValue, 0, 1023, 0, 100);
  return lightPercentage;
}

void createCSVFile()
{
  Serial.println("Creating CSV file...");
  File dataFile = SD.open("sensor_data.csv", FILE_WRITE);
  if (dataFile)
  {
    dataFile.println("Temperature,Humidity,Light,CO2,HumidityRatio");
    dataFile.close();
    Serial.println("CSV file created.");
  }
  else
  {
    Serial.println("Error creating CSV file!");
  }
}

void appendDataToCSV(int temperature,int humidity ,int CO2, int light, float HumidityRatio)
{
  File dataFile = SD.open("sensor_data.csv", FILE_WRITE);
  if (dataFile)
  {
    String dataString = String(millis()) + "," + String(temperature) +"," + String(humidity) + "," + String(CO2) + "," + String(light)+ "," + String(HumidityRatio);
    dataFile.println(dataString);
    dataFile.close();
  }
  else
  {
    Serial.println("Error appending data to CSV file!");
  }
}