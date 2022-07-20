avrdude -v -pattiny25 -cusbasp -e -Uefuse:w:0xFF:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xC4:m -Uflash:w:ATTiny25_LP_Blink_PORTB.ino.hex:i -B 16kHz
