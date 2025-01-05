# TD5580A
ESP32 Arduino library for TD5580A chip used in MP3 player devices

1. Adapted from GD3300 library repo https://github.com/misa3L994/GD3300/
2. Tuda Semi TD5580A reference PDF  http://www.tudasemi.com/static/upload/file/20240905/1725499313437991.pdf
3. Useful guide to common MP3 player devices  https://bigsoundbank.com/articles/df-player-mini-f104.html

This is a WORK IN PROGRESS.  Commands are currently being tested.  Queries are COMPLETELY untested.

```
rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:1184
load:0x40078000,len:13232
load:0x40080400,len:3028
entry 0x400805e4
Entered setup()...
Starting DFPlayer serial comms...
Beginning DFPlayer serial comms...
Selecting SD card as source...
Sending: 0X7e 0Xff 0X06 0X09 0X00 0X00 0X02 0Xef
Setting volume to 20...
Sending: 0X7e 0Xff 0X06 0X06 0X00 0X00 0X14 0Xef
Playing track # 3
Sending: 0X7e 0Xff 0X06 0X03 0X00 0X00 0X03 0Xef
```
