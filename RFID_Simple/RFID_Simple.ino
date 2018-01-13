#include <SoftwareSerial.h>
#define STX 2
#define ETX 3
#define DATA_LENGTH 14

SoftwareSerial RfidReader(10, 9);
String cardNumber;

void setup()
{
    Serial.begin(9600);
    RfidReader.begin(9600);
}

void loop()
{
    char num;
    
    if (RfidReader.available() > 0){
        num = RfidReader.read();
        if(num == STX){  //card number start
        	cardNumber = "";
        } 
        else if(num == ETX){ //card number end
            Serial.println(cardNumber);
        } else {
        	cardNumber += num;
        }
    }
}
