/*
 Created by Rui Santos
 Visit: http://randomnerdtutorials.com for more arduino projects

 Arduino with Ethernet Shield
 */

#include <SPI.h>
#include <Ethernet.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 172, 16, 0, 178 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 1, 1};                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
String readString;
String encryptedString;

char MasterKeyEnc[5];  //MasterKey is randomized key from the client
String PrivateKey = "crF&"; //will be the private key and can be any alphanumeric combination but the server and client must use the same PrivateKey.
char v[5];
byte flag = 0;
int hr; // hours
int mi; //minutes
int da; //day
int yr; //year
int mn; //month
int t; //combination of hr and mi (time)
int y; //combination of mn da and yr 


void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet) 
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(A0, OUTPUT);
digitalWrite(3, LOW);    
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
          
           
//We want just the Chiper nothing else from the readString
String temp1 = readString.substring(readString.indexOf("?")+1);
Serial.print("Temp1 pass1: ");
Serial.println(temp1);
temp1.remove(temp1.indexOf(" "));
encryptedString = temp1;
Serial.print("Temp1: ");
Serial.println(temp1);
Serial.print("EncryptedString: ");
Serial.println(encryptedString);
//Now we need to get the PublicKey from the encryptedString
sscanf(encryptedString.c_str(), "%04s%04s%02i%04i", &MasterKeyEnc, &v, &t, &y);
Serial.print("Master/Public Key: ");
Serial.println(MasterKeyEnc);
Serial.print("Encrypted Message: ");
Serial.println(v);
Serial.print("Master Lock Code: ");
Serial.print(t);
Serial.print(" ");
Serial.println(y);
hr = hour();
mi = minute();
da = day();
yr = year();
mn = month();
int t1; 
t1 += hr;
t1 += mi;
//Serial.print("Debug Mode: t1 after adding ");
//Serial.println(t1);
//Serial.println(t);
int y1; 
y1 += mn;
y1 += da;
y1 += yr;
//Serial.print("Debug Mode: y1 after adding ");
//Serial.println(y1);
//Serial.println(y);
if (t1+y1 == t+y) {
String decrypt = Vigenere_decrypt(v,PrivateKey);
Serial.print("Decrypted Message: ");
Serial.println(decrypt);
           
           //controls the Arduino if you press the buttons
           if (decrypt == "2 on"){
               digitalWrite(2, HIGH);
               client.println("<br \>LED 2 ON!<br \>");
               flag = 1;
           }
           if (decrypt == "2off"){
               digitalWrite(2, LOW);
               client.println("<br \>LED 2 OFF!<br \>");
               flag = 1;
           }
           
           if (decrypt == "3 on"){
               digitalWrite(A0, HIGH);
               client.println("<br \>LED 3 ON!<br \>");
               flag = 1;
           }
           if (decrypt == "3off"){
               digitalWrite(A0, LOW);
               client.println("<br \>LED 3 OFF!<br \>");
               flag = 1;
           }
} 
          if (flag == 0) {
            client.println("<br \><H1>Unauthorized!</H1><br \>");
          }
                      //clearing string for next read
            readString="";  
            flag = 0;
            y1 =0;
            t1 =0;
            //stopping client
           client.stop();
         }
       }
    }
}
}

String Vigenere_decrypt(String text, String seed) {
  seed.trim();
  text.trim();
  String cipher_key = Vigenere_getcipher_key(seed);
  String dec;
  for(int i = 0; i < text.length(); i++)  {
    int X = (text[i]-32);
    int K = ((cipher_key[i%(cipher_key.length())])-32);
    int temp = ((X-K)+95)%95;
    dec += (char) (temp + 32);
  }
  return dec;
}

//*---Print to see the table
String Vigenere_table(String seed) {
  seed.trim();
  String cipher_key = Vigenere_getcipher_key(seed);
  for(int i = 0; i < 95; i++) {
    Serial.print((char) (i+32));
  }
  Serial.print("\n");
  for(int i = 0; i < cipher_key.length(); i++) {    
    for(int j = 0; j < 95; j++) {
      int temp = cipher_key[i] - 32;
      Serial.print((char)(((temp+j)%95)+32));
    }
    Serial.print("\n");
  }
}

//*---Generate Vegenere Cipher key 95 char from 4char
String Vigenere_getcipher_key(String seed) {
  seed.trim();
  int _vige[seed.length()];
  int _seed[seed.length()];
  String cipher;
  String Vigenere_key_Table = MasterKeyEnc;    
  while (Vigenere_key_Table.length()<95) {
    for(int i = 0; i<seed.length() ;i++) {
      _vige[i] = (int) (Vigenere_key_Table[i]);
      _seed[i] = (int) (seed[i]);
      _vige[i] = _vige[i] + _seed[i];
      if (_vige[i] > 126) { 
        _vige[i] = _vige[i]-95;
        if (_vige[i] > 126) {
          _vige[i] = _vige[i]-95; 
        }
      }
      cipher += (char) (_vige[i]);
      seed.replace((char)(seed[i]), (char)(_vige[i]));
    } 
    Vigenere_key_Table += cipher;
    cipher = "";
  }
  if(Vigenere_key_Table.length() > 95) {
    Vigenere_key_Table.remove (Vigenere_key_Table.length()-(Vigenere_key_Table.length() - 95),Vigenere_key_Table.length());
  }
  return Vigenere_key_Table;
}


