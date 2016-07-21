/*
 * Hi, I create this Vigenere Encryption Code,
 * it have the ability to create a 95 random table key from 4 character
 * which are important due to the fact that the table key need to be always 
 * changed periodically, thus programmer can send small key to generate large 
 * key set
 * 
 * I create with a motivation to create a method of transfering data encryptedly 
 * for WSN and IoT application
 * 
 * Email me: shaiful90experiment@gmail.com
 * create---6/18/2016
 * based on Cifra work by Matthew--- , 7/26/2013
 */

/* July 12, 2016 - Added a PassKey variable
 *  LeRoy F. Miller - Automation Technology Club
 * July 20, 2016 added RTC DS3232 
 */

#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)


String MasterKeyEnc;  //MasterKey is randomized and becomes public
String PrivateKey = "crF&"; //will be the private key and can be any alphanumeric combination
char randomKey[5];
char l1;
char l2;
char l3;
char l4;
char SendString[27];
int hr; // hours
int mi; //minutes
int da; //day
int yr; //year
int mn; //month
int t; //combination of hr and mi (time)
int y; //combination of mn da and yr 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
randomSeed(analogRead(0));
l1 = ranletter();
l2 = ranletter();
l3 = ranletter();
l4 = ranletter();
randomKey[0] = l1;
randomKey[1] = l2;
randomKey[2] = l3;
randomKey[3] = l4;
  MasterKeyEnc = randomKey;
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet) 
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");
hr = hour();
mi = minute();
da = day();
yr = year();
mn = month();
t += hr;
t += mi;
y += mn;
y += da;
y += yr;
 
  String v = Vigenere_encrypt("2off",PrivateKey); 
  Serial.print("Random Key: ");
  Serial.println(MasterKeyEnc);
  Serial.print("Encryped Message: ");  
  Serial.println(v);
  Serial.print("Decrypted Message: ");
  Serial.println(Vigenere_decrypt(v,PrivateKey));
  Serial.print("Encryption Time: ");
  Serial.print(hr);
  Serial.print(mi);
  Serial.print(mn);
  Serial.print(da);
  Serial.println(yr);
 Serial.print("Combination Lock: ");
 Serial.print(t);
 Serial.print(" ");
 Serial.println(y);
  //Serial.println(Vigenere_getcipher_key(PrivateKey));
  sprintf(SendString, "%03s%03s%02i%04i", MasterKeyEnc.c_str(), v.c_str(),t,y);
  Serial.print("Send this: ");
  Serial.println(SendString);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}


String Vigenere_encrypt(String text, String seed) {
  seed.trim();
  text.trim();
  String cipher_key = Vigenere_getcipher_key(seed);
  String enc;
  for(int i = 0; i < text.length(); i++) {
    int X = (text[i]-32);
    int K = ((cipher_key[i%(cipher_key.length())])-32);
    int temp = (X+K)%95;
    enc += (char) (temp + 32);
  }
  return enc;
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

char ranletter() {
  byte randomValue = random(0, 37);
  char letter = randomValue + 'a';
  if (randomValue > 26) {letter = (randomValue - 26)+'0';}
  return letter;
}

