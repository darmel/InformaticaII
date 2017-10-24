  int p_rev; //pulsos por revolucion
  int rev_capa=5; // revoluciones por capa
  int n_espiras=500; //cantidad total de espiras
  int feedrate=100; //inicializado para referenciar
  unsigned long pulsos; //contador de pulsos del encoder
  int pulsos_rev; //cantidad de pulsos por revolucion
  long int rec=0; //variable para guardar lo que recibo
  long int check=0;
  int sentido = 4; //pin de control de motor PaP
  int clk = 7; //pin de clock para motor PaP
  int testled=13; //led onboard para pruebas
  int n=0; // variable para pruebas
 
  int contador=0;
  int ref=0; //guardo el estado de la referenciada
  int fcarrera = 8; //pin del final de carrera
  int fcarreraest = 0; //variable donde guardo el estado del final de carrera
  int motor=9; //motor de carretel
  const int ledPin =  13;
  
  int op=0; //opcion en el receving data
  int q=1; //variable para enviar en el check, para enviar intiguer y no char
  
void setup() {
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  pinMode(sentido, OUTPUT);
  pinMode(clk, OUTPUT);
  attachInterrupt( 0, encoder, FALLING); // configuramos la interrupcion como falling
  pinMode(fcarrera, INPUT);
  pinMode(testled, OUTPUT);
  op=2;
 
  while(n<5)
    {
      digitalWrite(testled, HIGH);
      delay(200);
      digitalWrite(testled, LOW);
      delay(200);
      n++;
    }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  
  //Serial.println(op);
  if(op==4)
  {
  Serial.print("feedrate="); Serial.write(feedrate);
  Serial.println("_");
  Serial.print("rev capas="); Serial.write(rev_capa);
  Serial.println("_");
  Serial.print("n espiras="); Serial.write(n_espiras);
  //op=0;
  }
  

}


//*************************/funcion serial event
void serialEvent() 
  {
  while (Serial.available())
      {
        rec = (long int)Serial.read();
        if(rec==q)
          {
            switch (op)
              {
                case 2:
                  feedrate=check;
                  break;
                case 3:
                  rev_capa=check;
                  break;
                case 4:
                  n_espiras=check;
                  break;
              }
          Serial.write(q);
          digitalWrite(testled, HIGH);
          op++;
          }
          else
            {
              check=rec;
              Serial.write(check);
            }
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
