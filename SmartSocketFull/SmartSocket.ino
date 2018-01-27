#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#define STX 2
#define ETX 3
#define DATA_LENGTH 14

const int chipSDCard = 8;
const int chipRelay2 = 5; // to save a jumper :)
const int chipRelay = 6;
const String fileName = "users.csv";
const String SEPARATOR = ";";
const int doubleReadDelay = 1000; 
const int OFF = HIGH;
const int ON = LOW;

SoftwareSerial RfidReader(2, 3);
String cardNumber, oldCard;

unsigned long time = 0;
int relayState;

void setup()
{
    Serial.begin(9600);
    if (!initializeSD()){
       return; 
    }
    RfidReader.begin(9600);
    time = millis();

    pinMode(chipRelay, OUTPUT); 
    pinMode(chipRelay2, OUTPUT); 
    relayState = OFF; // turned off
    oldCard = "";
    digitalWrite(chipRelay, relayState); 
    digitalWrite(chipRelay2, relayState); 
}

void loop() {
    if (RfidReader.available() > 0){
        char num = RfidReader.read();
        if(num == STX){  //card number start
            cardNumber = "";
        } 
        else if(num == ETX){ //card number end

            String correctLine = getUserLine(cardNumber, fileName);
            if (correctLine != ""){
                switchRelay(cardNumber, correctLine);
            }
            else{
                Serial.println("Card ID: " + cardNumber + ", User is not found");
            }
        } else {
            cardNumber += num;
        }
    }
}

//---------------------------------------------------
void switchRelay(String card, String userRecord){

    if(millis() - time < doubleReadDelay){
        return;
    }
    Serial.println("Card ID: " + card);

    if(relayState == OFF){
        relayState = ON;
        digitalWrite(chipRelay, relayState);
        digitalWrite(chipRelay2, relayState);
        oldCard = card;
        Serial.println("hello " + getUserName(userRecord) + ", turning ON");
    } 
    else if(relayState == ON){
        if(card == oldCard){
            relayState = OFF;
            digitalWrite(chipRelay, relayState);
            digitalWrite(chipRelay2, relayState);
            oldCard = "";
            Serial.println("welcome back " + getUserName(userRecord) + ", turning OFF");
        } else {
            Serial.println("sorry, " + getUserName(userRecord) + ", device is busy");
        }
    }
    time = millis();
}

//-------------------------------------------------
bool initializeSD(){
        while (!Serial) {; }

        Serial.print("Initializing SD card...");
        if (!SD.begin(chipSDCard)){
            Serial.println("initialization failed!");
            return false;
        }
        Serial.println("initialization done.");
        return true;
}


//-------------------------------------------------
String getOneLine(File file){
    String line;
    while (file.available()){
            char value = file.read();
            //String val = String(value); 
            if(value == '\n'){
                break;
            }       
            if(value == '\r'){
                continue;
            }     
            line += String(value);
    }
    return line;
}


//-------------------------------------------------
String getUserLine(String code, String usersFileName){
    File usersFile;
    String line = "";
    String cardCode = code + SEPARATOR;
    
    usersFile = SD.open(usersFileName);
    if (!usersFile){
        Serial.println("error opening " + usersFileName);
        return;
    }

    while(usersFile.available()){
        line = getOneLine(usersFile);
        if(line.indexOf(cardCode) == 0){
            break;
        }
        line = "";
    }

    usersFile.close();
    return line;
}


//-------------------------------------------------
String getUserName(String userLine){
    int place1 = userLine.indexOf(SEPARATOR) + 1;
    int place2 = userLine.indexOf(SEPARATOR, place1);
    String userName = userLine.substring(place1, place2);
    return userName;
}


//-------------------------------------------------
String getUserEmail(String userLine){
    int place1 = userLine.indexOf(SEPARATOR) + 1;
    int place2 = userLine.indexOf(SEPARATOR, place1) + 1;
    String userEmail = userLine.substring(place2);
    return userEmail;
}