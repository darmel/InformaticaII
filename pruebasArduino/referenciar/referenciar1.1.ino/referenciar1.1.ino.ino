  int p_rev; //pulsos por revolucion
  int rev_capa; // revoluciones por capa
  int n_espiras; //cantidad total de espiras
  unsigned long pulsos; //contador de pulsos del encoder
  int pulsos_rev; //cantidad de pulsos por revolucion
  int sentido = 4; //pin de control de motor PaP
  int clk = 7; //pin de clock para motor PaP
  int n=0; // variable para pruebas
  int feedrate=100; //inicializado para referenciar
  int contador=0;
  int ref=0; //guardo el estado de la referenciada
  int fcarrera = 8; //pin del final de carrera
  int fcarreraest = 0; //variable donde guardo el estado del final de carrera
  int motor=9; //motor de carretel
  const int ledPin =  13;
  long int rec=0;

void setup() {
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  pinMode(sentido, OUTPUT);
  pinMode(clk, OUTPUT);
 // attachInterrupt( 0, encoder, FALLING); // configuramos la interrupcion como falling
  pinMode(fcarrera, INPUT);
  digitalWrite(sentido, LOW);

    pinMode(ledPin, OUTPUT);

    while(n<5)
    {
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      delay(200);
      n++;
    }
    
    delay(3000);
digitalWrite(ledPin, HIGH);
    
fcarreraest = digitalRead(fcarrera); 
}

void loop() {
  // put your main code here, to run repeatedly:
 
  if(ref==0)
      {
        referenciar();      
      }



}

/*----------funcion referenciar------------*/
void referenciar()
  {
    fcarreraest = digitalRead(fcarrera); 
    digitalWrite(sentido, HIGH);
    while (fcarreraest==LOW)
      {
        digitalWrite(clk, !digitalRead(clk));
        delay(5);
        fcarreraest = digitalRead(fcarrera); 
        contador++;   
      }
      
    ref=1;
    digitalWrite(sentido, LOW);
  }

void serialEvent() 
  {
  while (Serial.available())
      {
        rec = (long int)Serial.read();
      }
    if (rec=='q')
      Serial.println(contador);
    if(rec=='w')
      contador=0;
    if(rec=='d')
      n=contador;
    if(rec=='e')
      Serial.println(n);
      
  }


