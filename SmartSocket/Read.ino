#include <SPI.h>
#include <SD.h>

const String SEPARATOR = ";";
const int chipSelect = 10;
const String fileName = "users.csv";
const String cardCode = "627" + SEPARATOR;

void setup()
{
    if (!initializeSD()){
       return; 
    }

    String correctLine = getUserLine(cardCode,fileName);
    if (correctLine != "")
    {
    Serial.println("hello " + getUserName(correctLine));
    Serial.println("I will mail you to: " + getUserEmail(correctLine));    
    }
    else{
        Serial.println("User is not found");
    }
}

void loop()
{
    // nothing happens after setup
}

//-------------------------------------------------
String getOneLine(File file){
    String line;
    while (file.available()){
            char value = file.read();
            String val = String(value); 
            if(value == '\n'){
                break;
            }       
            if(value == '\r' ){
                continue;
            }     
            line += val;
    }
    return line;
}


//-------------------------------------------------
String getUserLine(String code, String usersFileName){
    File usersFile;
    String line = "";

    
    usersFile = SD.open(usersFileName);
    if (!usersFile){
        Serial.println("error opening " + usersFileName);
        return;
    }

    while(usersFile.available()){
        line = getOneLine(usersFile);
        if(line.indexOf(code)==0){
            break;
        }
        line = "";
    }

    usersFile.close();
    return line;
}


//-------------------------------------------------
bool initializeSD(){
        Serial.begin(9600);
        while (!Serial) {
            ; 
        }
        Serial.print("Initializing SD card...");
        if (!SD.begin(chipSelect))
        {
            Serial.println("initialization failed!");
            return false;
        }
        Serial.println("initialization done.");
        return true;
}


//-------------------------------------------------
String getUserName(String userLine){
    int place1 = userLine.indexOf(SEPARATOR)+1;
    int place2 = userLine.indexOf(SEPARATOR,place1);
    String userName;
    userName = userLine.substring(place1,place2);
    return userName;
}


//-------------------------------------------------
String getUserEmail(String userLine){
    int place1 = userLine.indexOf(SEPARATOR)+1;
    int place2 = userLine.indexOf(SEPARATOR,place1)+1;
    String userEmail;
    userEmail = userLine.substring(place2);
    return userEmail;
}