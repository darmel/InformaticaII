/*
  Serial Event example

 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and
 clears it.

 A good test for this is to try it with a GPS receiver
 that sends out NMEA 0183 sentences.

 Created 9 May 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialEvent

 */

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char caracter;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);
   Serial.println("comienzo");
   pinMode(8, INPUT);
   pinMode(9, INPUT);
}
int l=0;
int b[] = {14, 15, 18,} ;
int a=15;
int b1 = 0;
int b2 = 0;
int b3 = 0;
void loop() {
  // print the string when a newline arrives:
     // clear the string:

       if (l==0)
      digitalWrite(13, HIGH);
      else
      digitalWrite(13, LOW);

  if(b2==0)
  {
    switch (caracter) {
    case 'a':
      Serial.println("a");
    break;
    case 'b':
      Serial.println("q");
      break;
      
    case 'c': 
      Serial.write(b)
    break;
  }
         b2==1;
  }
      
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char carater = (char)Serial.read();
     b2=0;
    
    Serial.println("caracter recibido");
    Serial.println(inputString);
    
    if (l==0)
      l=1;
      else
      l=0;
 
    
  }
}


