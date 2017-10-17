  int p_rev; //pulsos por revolucion
  int rev_capa=5; // revoluciones por capa
  int n_espiras=500; //cantidad total de espiras
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
  long int rec=0; //variable para guardar lo que recibo
  long int check=0;
  int op=0; //opcion en el receving data
  
void setup() {
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  pinMode(sentido, OUTPUT);
  pinMode(clk, OUTPUT);
  attachInterrupt( 0, encoder, FALLING); // configuramos la interrupcion como falling
  pinMode(fcarrera, INPUT);
  op=0;
  //delay(1000);
 // Serial.println(op);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Serial.println(op);
  if(op==4)
  {
  Serial.print("feedrate="); Serial.write(15);
  Serial.println("_");
  Serial.print("rev capas="); Serial.write(rev_capa);
  Serial.println("_");
  Serial.print("n espiras="); Serial.write(n_espiras);
  op=0;
  }
  

}


//*************************/funcion serial event
void serialEvent() 
  {
  while (Serial.available())
      {
        rec = (long int)Serial.read();
        Serial.write(rec);
        if(rec!=1)
        check=rec;
        if(rec==1)
          {
            op++;
            Serial.write(rec);
          }
      }
    
    switch (op) 
  {
    case 3:
      n_espiras = check;
      break;
    case 2:
      rev_capa = check;
      break;
    case 1:
      feedrate = check;
     // Serial.print("feedrate="); Serial.write(15);
     // Serial.println("_");
      break;
      
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }
    
    
  
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
