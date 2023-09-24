#include <SPI.h>              // include libraries
#include <LoRa.h>

#define ss 4
#define rst 14
#define dio0 2

 
byte MasterNode = 0xFF;     
byte Node1 = 0xBB;
//to add another node,add more address like this,eg: byte Node2 = 0xCC;
 
String SenderNode = "";
String outgoing;//Outgoing message 
byte msgCount = 0;//count of outgoing messages
unsigned long previousMillis=0;
unsigned long int previoussecs = 0; 
unsigned long int currentsecs = 0; 
unsigned long currentMillis = 0;
int interval= 1 ; // updated every 1 second
int Secs = 0; 
 
void setup() {
  // Initialize Serial Monitor
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
 
 
currentMillis = millis();
   currentsecs = currentMillis / 1000; 
    if ((unsigned long)(currentsecs - previoussecs) >= interval) {
     Secs = Secs + 1;
     //Serial.println(Secs);
     if ( Secs >= 11 )
    {
      Secs = 0; 
    }
    if ( (Secs >= 1) && (Secs <= 5) )
    {
     
    String message = "34"; 
    sendMessage(message,MasterNode, Node1);//To communicate with other node
    String msg1=Serial.readString(); //Enter the data to be sent
    sendMessage(msg1,MasterNode, Node1);
    
    }
 
        if ( (Secs >= 6 ) && (Secs <= 10))
    {
     
    String message = "55"; 
    sendMessage(message,MasterNode, Node2);
    }
    
   previoussecs = currentsecs;
    }
 
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
    
  }
 
 
void sendMessage(String outgoing, byte MasterNode, byte otherNode) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(otherNode);              // add destination address
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
  if( sender == 0XBB )
  SenderNode = "Node1:";
  //uncomment below lines if using node 2
  //if( sender == 0XCC )  
  //SenderNode = "Node2:";
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
 
  String incoming = "";
 
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
 
  if (incomingLength != incoming.length()) {   // check length for error
    //Serial.println("error: message length does not match length");
    ;
    return;                             // skip rest of function
  }
 
  // if the recipient isn't this device or broadcast,
  if (recipient != Node1 && recipient != MasterNode) {
   // Serial.println("This message is not for me.");
    ;
    return;                             // skip rest of function
  }
 
  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
 
  Serial.println(SenderNode);
 if( sender == 0XBB )
 { 
  Serial.println(incoming + "Pot");
 }
 
  if( sender == 0XCC )
 { 
  Serial.println(incoming + "C");
 }
 
Serial.println(); 
}
