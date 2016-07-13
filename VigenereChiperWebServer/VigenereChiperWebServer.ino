/*
 Created by Rui Santos
 Visit: http://randomnerdtutorials.com for more arduino projects

 Arduino with Ethernet Shield
 */

#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 1, 178 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 1, 1};                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
String readString;
String encryptedString;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
    
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
         //for (int xx=0; xx<100; xx++) {
          //if (xx > 7) {
          //store characters to string
                    readString += c;
          //}
                    //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
     
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           //client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           //client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           //client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>Project Turn on Stuff From Website</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>Turn on Stuff from a Website </H1>");
           client.println("<H1>With Vigenere Chiper Encryption</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("Turn LED on Pin 2 - On/Off = 2on or 2off");
           client.println("<br />");
           client.println("Turn LED on Pin 3 - On/Off = 3on or 3off");
           client.println("<br />"); 
           client.println("<hr />");
           client.println("<p>Based on a sketch. Web Server");
           client.println("<br />Created by Rui Santos. Visit http://randomnerdtutorials.com for more projects!");  
           client.println("<br />Modified to use a Vigenere Cipher by LeRoy F. Miller 2016.");
           client.println("<br />Automation Technology Club - West Chester, Ohio");
           client.println("<br />");
           client.println("https://github.com/automation-technology-club/ArduinoVigenereCipher/tree/ourchanges");
           client.println("<br />Vigener Cipher based on ShaifulFlame ArduinoVigenereCipher.");
           client.println("<br />https://github.com/ShaifulFlame/ArduinoVigenereCipher");
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           //stopping client
           client.stop();
           
//We want just the Chiper nothing else from the readString
String temp1 = readString.substring(readString.indexOf("?")+1);
temp1.remove(temp1.indexOf(" "));
encryptedString = temp1;
Serial.print("EncryptedString: ");
Serial.println(encryptedString);
//Now we need to get the PublicKey from the encryptedString

           
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?button1on") >0){
               digitalWrite(2, HIGH);
           }
           if (readString.indexOf("?button1off") >0){
               digitalWrite(2, LOW);
           }
           
           if (readString.indexOf("?button2on") >0){
               digitalWrite(3, HIGH);
           }
           if (readString.indexOf("?button2off") >0){
               digitalWrite(3, LOW);
           }
           
           if (readString.indexOf("?button3on") >0){
               digitalWrite(4, HIGH);
           }
           if (readString.indexOf("?button3off") >0){
               digitalWrite(4, LOW);
           }
           
           
           if (readString.indexOf("?button4on") >0){
               digitalWrite(5, HIGH);
           }
           if (readString.indexOf("?button4off") >0){
               digitalWrite(5, LOW);
           }
           
           if (readString.indexOf("?button5on") >0){
               digitalWrite(6, HIGH);
           }
           if (readString.indexOf("?button5off") >0){
               digitalWrite(6, LOW);
           }
           
           if (readString.indexOf("?button6on") >0){
               digitalWrite(7, HIGH);
           }
           if (readString.indexOf("?button6off") >0){
               digitalWrite(7, LOW);
           }
           
           /*
           if (readString.indexOf("?7on") >0){
               digitalWrite(8, HIGH);
           }
           if (readString.indexOf("?7off") >0){
               digitalWrite(8, LOW);
           }
           
           if (readString.indexOf("?8on") >0){
               digitalWrite(9, HIGH);
           }
           if (readString.indexOf("?8off") >0){
               digitalWrite(9, LOW);
           }
           */
           
                      //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}
