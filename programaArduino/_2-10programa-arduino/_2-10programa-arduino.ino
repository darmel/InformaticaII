  int p_rev; //pulsos por revolucion
  int rev_capa; // revoluciones por capa
  int n_espiras; //cantidad total de espiras
  int pulsos; //contador de pulsos del encoder
  int pulsos_rev; //cantidad de pulsos por revolucion

void setup() {
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);


}

void loop() {
  // put your main code here, to run repeatedly:

}

//funcion serial event
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

//funcion referenciar
void referenciar()
  {
  }
  }

