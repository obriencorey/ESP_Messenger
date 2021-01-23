# ESP_Messenger

![Image of Messenger Wiring](messagewiring1.jpg)

This project was born out of the increase in technology, yet decrease in human interaction caused by the COVID-19 pandemic. Texts and emails just didn't feel as personal, and some issues with the United States Mail System motivated me to invent my own form of messenging. Based around a PHP script implemented on my personal website, this program also uses an encryption program I wrote (It is not secure, it was just merely to make me feel like a capable hacker)


## How To Install
While this project was mainly for personal use, I figured maybe some of the code could be of some use.

### Download
Download repository using method of choice. Place everything into a folder called 
```
ESP8266_MSG
```
Place the following files inside.
```
"ESP8266_MSG.ino", 
"ImageData.c", 
"ImageData.h" 
```
### Place Folders in Arduino Folder
Place the following folder
```
"esp8266-waveshare-epd"
```
inside the following filepath
```
"C:\Users\{YOURUSERHERE}\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\{VERSIONNUMBER}\libraries\esp8266-waveshare-epd"
```
### Update Wifi Credentials
In ESP8266_msg.ino, add your wifi SSID and password and then upload.

Also make sure the board is “NodeMCU 1.0 (ESP-12E Module)”

You might need to install “ESPWiFi.h” and “ESP8266HTTPClient.h” as well, but you can do that from the library manager I believe.

