#include <SPI.h>
#include <LoRa.h>

#define ss 4
#define rst 14
#define dio0 2


String outgoing;              // outgoing message
 
byte msgCount = 0;            // count of outgoing messages
byte MasterNode = 0xFF;     
byte Node1 = 0xBB;
const int ledPin = 14;  

void setup() {
  // Initialize Serial Monitor
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  // Setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  // Replace the LoRa.begin(---E-) argument with your location's frequency
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
  // 866 MHz to 867 MHZ is licensed in India
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the transmitter
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}



void loop() {
 
 
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
  
}
 
void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(MasterNode);              // add destination address
  LoRa.write(Node1);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

  void sendMessage1(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(Node1);              // add destination address
  LoRa.write(MasterNode);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}
 
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
 
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
 
  String incoming = "";
 
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
 
  if (incomingLength != incoming.length()) {   // check length for error
   // Serial.println("error: message length does not match length");
   ;
    return;                             // skip rest of function
  }
 
  // if the recipient isn't this device or broadcast,
  if (recipient != Node1 && recipient != MasterNode) {
    //Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }

    Serial.println(incoming);
    int Val = incoming.toInt();
    
    if(Val==1) // if val is 1 then switch on LED
    {
      analogWrite(ledPin,HIGH);
    Serial.println("LED ON");
    }
    if(Val==2) // if val is 2, then switch off LED
    {
          analogWrite(ledPin,LOW);
          Serial.println("LED OFF");
    }
    if(Val == 34)
    { Serial.println("Enter a message to send:");
    String command = Serial.readString();
    Serial.println("Enter a command to send:");
    sendMessage1(command,MasterNode,Node1);
    delay(100);
    } 
}
