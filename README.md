# Thermohygrometer
Thermohygrometer using DHT11 and SHT30 sensor, Wemos d1 mini (ESP8266) and OLED(64x48)

The source tree has 4 sub-directory.
3 are using DHT11 temperature-humidity sensor and the last one is using SHT30 temperature-humidity sensor.
As the name shows, you can use one for the type of your network connection.

<a> <img alt="Pic" src="https://github.com/nkkim91/Thermohygrometer/blob/master/pictures/Thermohygrometer1.JPG"https://github.com/nkkim91/Thermohygrometer/blob/master/pictures/Thermohygrometer1.JPG> </a>


## Feature
- The device connects to the local network and send the temperature-humidity data to the mysql server.
- The device itselfs works as web-server and shows the temperature-humidity information through the web-page
- The date/time information are syncted to timer server and updated repeatedly.
- The device has OLED (64x48) and shows the information such as date/time, temperature/humidity and ip address of the device.

## 3D design file is accessible at tinkercad.
- https://www.tinkercad.com/things/d1Y03uMj6P0 
