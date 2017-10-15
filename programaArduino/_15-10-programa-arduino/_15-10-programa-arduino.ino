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
  int rec=0; //variable para guardar lo que recibo
  
void setup() {
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  pinMode(sentido, OUTPUT);
  pinMode(clk, OUTPUT);
  attachInterrupt( 0, encoder, FALLING); // configuramos la interrupcion como falling
  pinMode(fcarrera, INPUT);

}

void loop() 
{
  // put your main code here, to run repeatedly:

   delay(1000);
   fcarreraest = digitalRead(fcarrera);
   // Serial.println(fcarreraest); //muestra el estado de fcarreraest. eliminar cuando todo funcione
    if(ref==0)
      {
        referenciar();      
      }


    
    if (n != contador) //muestra los pulsos del encoder
            {     Serial.println(contador);
                  n = contador ;
            }
  


    digitalWrite(sentido, HIGH); //le vuelvo el sentido HIGH al motor PaP     
}


/*---------------------------------------*/
void encoder() //funcion que suma uno a <<pulsos>> con cada falling en el pin 2
   {    
    contador++ ;
    if(contador==feedrate)
      {
        digitalWrite(clk, !digitalRead(clk));
        contador=0;
      }
   }
  

//*************************/funcion serial event
void serialEvent() 
  {
  while (Serial.available())
      {
       rec = (int)Serial.read(); 
      }
  Serial.write(rec+4);
  }

/*----------funcion referenciar------------*/
void referenciar()
  {
    Serial.println("referenciar");
     Serial.println(fcarreraest);
    while (fcarreraest==HIGH)
      {
        digitalWrite(sentido, LOW);
        fcarreraest = digitalRead(fcarrera);
            Serial.println("sentido LOW");
      }
    ref=1;
    Serial.println("referenciado");
  }
 




