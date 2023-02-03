#include <SPI.h> // Biblioteca SPI já incluída no Arduino IDE
#include "Wire.h" // Biblioteca Wire já incluída no Arduino IDE
#include <MFRC522.h> // Biblioteca MFRC522 https://github.com/miguelbalboa/rfid
#include "RTClib.h" // Biblioteca RTClib https://github.com/adafruit/RTClib
#include <TinyGPS++.h> // Biblioteca TinyGPS++ http://arduiniana.org/libraries/tinygpsplus/

#define SS_PIN 21  // SS do módulo RFID
#define RST_PIN 22 // RST do módulo RFID
#define RXD2 16    // RXD2 do módulo GPS
#define TXD2 17    // TXD2 do módulo GPS

TinyGPSPlus gps;               // GPS
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;       // Chave para autenticaçào
RTC_DS3231 rtc;                // RTC

byte nuidPICC[4]; // NUID do cartão
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"};

void setup()
{
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // INICIA O GPS
  Serial.begin(9600);
  SPI.begin();       // Init SPI bus
  rfid.PCD_Init();   // Init MFRC522
  Wire.begin(15, 4); // SDA, SCL
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  if (!rtc.begin()) // SE O RTC NÃO FOR INICIALIZADO, FAZ
  {                                          
    Serial.println("DS3231 não encontrado");
    while (1)
      ; // SEMPRE ENTRE NO LOOP
  }
  if (rtc.lostPower()) // SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
  {                               
    Serial.println("DS3231 OK!");
    // REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    // rtc.adjust(DateTime(2018, 9, 29, 15, 00, 45)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  delay(100);
}

void loop()
{
  gps.encode(Serial2.read());
  if (!rfid.PICC_IsNewCardPresent()) // Redefina o loop se nenhum novo cartão estiver presente no sensor/leitor. Isso salva todo o processo quando ocioso.
    return;

  if (!rfid.PICC_ReadCardSerial()) // Verifica se o NUID foi lido com sucesso
    return;

  for (byte i = 0; i < 4; i++) // Armazena NUID no array nuidPICC
  {
    nuidPICC[i] = rfid.uid.uidByte[i]; // Copia o NUID
  }

  Serial.println(F("The NUID tag is:")); // Imprime o NUID no monitor serial
  Serial.print(F("In hex: ")); 
  printHex(rfid.uid.uidByte, rfid.uid.size); 
  Serial.println();
  Serial.print(F("In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size); 
  Serial.println();

  DateTime now = rtc.now();                        // CHAMADA DE FUNÇÃO
  Serial.print("Data: ");                          // IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(now.day(), DEC);                    // IMPRIME NO MONITOR SERIAL O DIA
  Serial.print('/');                               // IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.month(), DEC);                  // IMPRIME NO MONITOR SERIAL O MÊS
  Serial.print('/');                               // IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.year(), DEC);                   // IMPRIME NO MONITOR SERIAL O ANO
  Serial.print(" / Dia: ");                        // IMPRIME O TEXTO NA SERIAL
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); // IMPRIME NO MONITOR SERIAL O DIA
  Serial.print(" / Horas: ");                      // IMPRIME O TEXTO NA SERIAL
  Serial.print(now.hour(), DEC);                   // IMPRIME NO MONITOR SERIAL A HORA
  Serial.print(':');                               // IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.minute(), DEC);                 // IMPRIME NO MONITOR SERIAL OS MINUTOS
  Serial.print(':');                               // IMPRIME O CARACTERE NO MONITOR SERIAL
  Serial.print(now.second(), DEC);                 // IMPRIME NO MONITOR SERIAL OS SEGUNDOS
  Serial.println();                                // QUEBRA DE LINHA NA SERIAL
  displayInfo();
  Serial.println();

  rfid.PICC_HaltA(); // Para o PICC
  rfid.PCD_StopCrypto1(); // Interrompe a criptografia no PCD
}

/**
 * Rotina auxiliar para despejar uma matriz de bytes como valores hexadecimais para Serial.
 */
void printHex(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Rotina auxiliar para despejar uma matriz de bytes como valores dec para Serial.
 */
void printDec(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void displayInfo() 
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid()) // SE A LOCALIZAÇÃO FOR VÁLIDA, FAZ
  {
    Serial.print(gps.location.lat(), 6); // IMPRIME A LATITUDE
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6); // IMPRIME A LONGITUDE
  }
  else
  {
    Serial.print(F("INVALID")); // SE NÃO, IMPRIME INVALID
  }
  Serial.println();
}
