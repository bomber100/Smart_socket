#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 6
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN); // объект MFRC522
unsigned long uidDec, uidDecTemp; // для отображения номера карточки в десятичном формате
byte bCounter, readBit;
unsigned long ticketNumber;
void setup()
{
    Serial.begin(9600);
    SPI.begin(); // инициализация SPI
    mfrc522.PCD_Init(); // инициализация MFRC522
    Serial.println("Waiting for card...");
}

void loop()
{
    // Поиск новой карточки
    if (!mfrc522.PICC_IsNewCardPresent()){
        return;
    }
    // Выбор карточки
    if (!mfrc522.PICC_ReadCardSerial()){
         return;
    }
    uidDec = 0;
    // Выдача серийного номера карточки
    Serial.print("Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++){
        uidDecTemp = mfrc522.uid.uidByte[i];
        uidDec = uidDec * 256 + uidDecTemp;
    }
    Serial.println(uidDec);

    // Выдача типа карты
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak); // запрос типа
    Serial.print("Card type: ");
    Serial.println(mfrc522.PICC_GetTypeName(piccType)); // трансляция типа в читаемый вид
    
    // Halt PICC
    mfrc522.PICC_HaltA();
}
