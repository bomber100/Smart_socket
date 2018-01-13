#include <SoftwareSerial.h>
#define STX 2
#define ETX 3
#define DATA_LENGTH 14

SoftwareSerial RFID(10, 9); // RX and TX
int rfidResult;
int rx_counter;
byte rx_data[DATA_LENGTH]; // 1+10+2+1
String cardNumber = ""; 
String oldCardNumber = ""; 

void setup()
{
    rx_counter = 0; // init counter
    RFID.begin(9600); // start serial to RFID reader
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600); // start serial to PC
}

void loop() {
    if (RFID.available() > 0) {
        rx_data[rx_counter] = RFID.read();
        if (rx_counter == 0 && rx_data[0] != STX) {
            Serial.println("Out of sync!"); // do not increment rx_counter
        } else {
            rx_counter++;
        }
        if (rx_counter >= DATA_LENGTH) {
            rx_counter = 0; // reset counter
            if (rx_data[0] == STX && rx_data[DATA_LENGTH-1] == ETX) {
                // packet starts with STX and ends with ETX
                byte calc_checksum = 0;
                for (int i = 0; i < 6; i++) {
                    // data with checksum
                    calc_checksum ^= ascii_char_to_num(rx_data[i * 2 + 1]) * 16 + ascii_char_to_num(rx_data[i * 2 + 2]);
                }
                if (calc_checksum == 0) {
                    cardNumber = "";
                    for (int i = 1; i <= 10; i++) {
                    	cardNumber += char(rx_data[i]);
                       // Serial.write(rx_data[i]);
                    }
                    if (oldCardNumber != cardNumber){
                    	Serial.print("ID: ");
                    	Serial.println(cardNumber);
                    	oldCardNumber = cardNumber;
                    	//blink(2);
                    }
                   
                } else {
                    Serial.println("Checksum ERROR!");
                }
            } else {
                Serial.println("Incorrect packet!");
            }
        }
    }
}

void blink(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(100); // wait for a half of second
        digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
        delay(100); // wait for a half of second
    }
}

// convert a single hex character to its byte value using ASCII table (see https://ru.wikipedia.org/wiki/ASCII)
byte ascii_char_to_num(byte a){
    a -= '0'; // 0..9
    if (a > 9)
        a -= 7; // A..F
    return a;
}
