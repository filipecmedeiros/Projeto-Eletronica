#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Ethernet.h>

#define BUZZER 22
#define SS_PIN 9
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
LiquidCrystal lcd(6, 7, 5, 4, 3, 2); 
 
char st[20];


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 15, 100);

EthernetClient client;
char server[] = "www.google.com";

void setup() 
{
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);   // Inicia a serial
  
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
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
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
    mensageminicial();
}
 
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "13 06 B8 C3") //UID 1 - Chaveiro
  {
    Serial.println();
    bip();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");
    delay(3000);
    mensageminicial();
  }
 
  if (conteudo.substring(1) == "D0 03 3C 25") //UID 2 - Cartao
  {
    Serial.println("Ola Cartao !");
    Serial.println();
    bip();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Cartao !");
    lcd.setCursor(0,1);
    lcd.print("Acesso Negado !");
    delay(3000);
    mensageminicial();
  }
} 
 
void mensageminicial()
{
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
