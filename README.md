# Pelletmeter

Arduino application and web service for a device that measures wood pellet level in an open silo.
The device should be mounted above the pellet surface and calculates level by measururing the
distance from the device to the surface.

## Arduino application

The Arduino device makes use of the an ESP8266 WiFi module and a HC-SR04 distance sensor. The
schematics are outlined in the file settings.h.tmpl. This file should be renamed settings.h
and edited to contain your WiFi credentials and server address.

## Web service

The web service is a simple Python Flask app that receives the measurements, evens them out,
converts them from distance to level and displays the result (level and a time line graph) on
a web page. It is located in the server/ folder. Adjust settings the settings.py file and upload
it to any Python-driven http server, e.g PythonAnywhere or similar.