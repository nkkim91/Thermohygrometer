# Thermohygrometer
Thermohygrometer using DHT11 and SHT30 sensor, Wemos d1 mini (ESP8266) and OLED(64x48)

The source tree has 4 sub-directory.
3 are using DHT11 temperature-humidity sensor and the last one is using SHT30 temperature-humidity sensor.
As the name shows, you can use one for the type of your network connection.

<a> <img src="https://github.com/nkkim91/Thermohygrometer/blob/master/pictures/Thermohygrometer1.JPG" width=320> </a>
<a> <img src="https://github.com/nkkim91/Thermohygrometer/blob/master/pictures/Thermohygrometer2.JPG" width=320> </a>
<a> <img src="https://github.com/nkkim91/Thermohygrometer/blob/master/pictures/Thermohygrometer3.JPG" width=320> </a>
<a> <img src="https://github.com/nkkim91/Thermohygrometer/blob/master/pictures/Thermohygrometer4.JPG" width=320> </a>
<a> <img src="https://github.com/nkkim91/Thermohygrometer/blob/master/pictures/Thermohygrometer5.JPG" width=320> </a>

## H/W Parts
- [DHT11 temperature/humidity sensor](https://www.aliexpress.com/item/32534235492.html?spm=a2g0s.9042311.0.0.27424c4d8OffY5)
- [SHT30 temperature/humidity sensor](https://www.aliexpress.com/item/33014223247.html?spm=a2g0o.productlist.0.0.595e1cdaaRN8tE&algo_pvid=b217e640-2794-4995-9487-2ca4cb381aa9&algo_expid=b217e640-2794-4995-9487-2ca4cb381aa9-48&btsid=0661026f-0b58-4375-9b2e-b1395b5d72f1&ws_ab_test=searchweb0_0,searchweb201602_2,searchweb201603_53)
- [OLED (64x48)](https://www.aliexpress.com/item/32627787079.html?spm=a2g0s.9042311.0.0.27424c4d8OffY5)
- [Wemos d1 mini](https://www.aliexpress.com/item/33040708716.html?spm=a2g0o.productlist.0.0.595e1cdaaRN8tE&algo_pvid=b217e640-2794-4995-9487-2ca4cb381aa9&algo_expid=b217e640-2794-4995-9487-2ca4cb381aa9-51&btsid=0661026f-0b58-4375-9b2e-b1395b5d72f1&ws_ab_test=searchweb0_0,searchweb201602_2,searchweb201603_53)
- [Wemos d1 mini triple base](https://www.aliexpress.com/item/32807833164.html?spm=a2g0s.9042311.0.0.27424c4d8OffY5)

## Feature
- The device connects to the local network and send the temperature-humidity data to the mysql server.
- The device itselfs works as web-server and shows the temperature-humidity information through the web-page
- The date/time information are syncted to timer server and updated repeatedly.
- The device has OLED (64x48) and shows the information such as date/time, temperature/humidity and ip address of the device.

## 3D design file is accessible at tinkercad.
- https://www.tinkercad.com/things/d1Y03uMj6P0 
