
/*Code Snipit by LeRoy Miller (2016) generate a random 4 char alphanumeric pass key
*Use how you wish, as this is just a snipit of a larger program
*you may or may not find useful for other things.
*July 13, 2016
 */


char randomKey[5];
char l1;
char l2;
char l3;
char l4;

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

Serial.print("Letter 1: ");
Serial.println(l1);
Serial.print("Letter 2: ");
Serial.println(l2);
Serial.print("Letter 3: ");
Serial.println(l3);
Serial.print("Letter 4: ");
Serial.println(l4);
Serial.print("Random Key: ");
Serial.println(randomKey);
String PassKey = randomKey;
Serial.print("String Key: ");
Serial.println(PassKey);
}


void loop() {
  // put your main code here, to run repeatedly:

}

char ranletter() {
  byte randomValue = random(0, 37);
  char letter = randomValue + 'a';
  if (randomValue > 26) {letter = (randomValue - 26)+'0';}
  return letter;
}

