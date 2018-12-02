#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Ethernet.h>

#define BUZZER 22
#define SS_PIN 9
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);

char st[20];

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 200);

EthernetClient client;
char server[] = "192.168.0.31";

void setup(){
    pinMode(BUZZER, OUTPUT);
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
  
    SPI.begin();      // Inicia  SPI bus
    mfrc522.PCD_Init();   // Inicia MFRC522
    //Define o número de colunas e linhas do LCD:  
    lcd.begin(16, 2);

    // start the Ethernet connection:
    Serial.println("Initialize Ethernet with DHCP:");
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      // Check for Ethernet hardware present
      if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        while (true) {
          delay(1); // do nothing, no point running without Ethernet hardware
        }
      }
      if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("Ethernet cable is not connected.");
      }
      // try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    } else {
      Serial.print("  DHCP assigned IP ");
      Serial.println(Ethernet.localIP());
    }
  
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();
    mensageminicial();
}
 
void loop() 
{
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()){
        return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()){
        return;
    }
    //Mostra UID na serial
    Serial.print("UID da tag :");
    String conteudo= "";
    byte letra;
    for (byte i = 0; i < mfrc522.uid.size; i++){
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Mensagem : ");
    conteudo.toUpperCase();
    //UID
    if (conteudo.substring(1) != ""){
        get_request(conteudo.substring(1));
        Serial.println();
        bip();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Presenca");
        lcd.setCursor(0,1);
        lcd.print("confirmada");
        delay(3000);
        mensageminicial();
    }
} 
  
 
void mensageminicial(){
    lcd.clear();
    lcd.print(" Aproxime o seu");  
    lcd.setCursor(0,1);
    lcd.print("cartao do leitor");  
}

void bip(){
    digitalWrite(BUZZER, HIGH);
    delay(400);
    digitalWrite(BUZZER, LOW);
}

void get_request(String UID){
    UID[2] = '-';
    UID[5] = '-';
    UID[8] = '-';
    Serial.println(UID);
    // give the Ethernet shield a second to initialize:
    delay(1000);
    Serial.print("connecting to ");
    Serial.print(server);
    Serial.println("...");
  
    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
      Serial.print("connected to ");
      Serial.println(client.remoteIP());
      // Make your API request:
      client.println("GET /" + UID + " HTTP/1.1");
      client.println("Host: 192.168.0.31");
      client.println("Connection: close");
      client.println();
    }
    else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
    }
}
