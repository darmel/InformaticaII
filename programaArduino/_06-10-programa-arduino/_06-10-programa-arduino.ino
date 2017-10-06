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
  int ref=0;
  int fcarrera = 8;
  int fcarreraest = 0; //variable donde guardo el estado del final de carrera
void setup() {
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);
  pinMode(sentido, OUTPUT);
  pinMode(clk, OUTPUT);
  attachInterrupt( 0, encoder, FALLING); // configuramos la interrupcion como falling
  pinMode(fcarrera, INPUT);

}

void loop() 
{
  // put your main code here, to run repeatedly:
  referenciar(); //cuando enciende lo primero que hace es referenciar
  
  if (n != contador)
          {     Serial.println(contador);
                n = contador ;
          }
  
  Serial.println(ref);
  if(ref=0)
  {
  referenciar(); //cuando enciende lo primero que hace es referenciar
  Serial.println(ref);
  }

        
}


//funcion serial event
void serialEvent() 
  {
  while (Serial.available())
      {
      }
  }

//funcion referenciar
void referenciar()
  {
    Serial.println("referenciar");
    fcarreraest = digitalRead(fcarrera);
    while (fcarreraest == HIGH) 
    {//vuelve al inicio
      digitalWrite(sentido, HIGH);
      fcarreraest = digitalRead(fcarrera);
      delay(1000);
    }
    Serial.println("referenciado");
    ref=1;
  }

void encoder() //funcion que suma uno a <<pulsos>> con cada falling en el pin 2
   {    
    contador++ ;
    if(contador==feedrate)
      {
        digitalWrite(clk, !digitalRead(clk));
        contador=0;
      }
   }
  

