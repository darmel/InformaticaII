  long int p_rev; //pulsos por revolucion
  long int rev_capa=5; // revoluciones por capa
  long int n_espiras=500; //cantidad total de espiras
  long int feedrate=100; //inicializado para referenciar
  unsigned long pul_encod; //contador de pulsos del encoder para las revolucion completa
  long int pulsos_rev; //cantidad de pulsos por revolucion
  long int rec=0; //variable para guardar lo que recibo
  long int check=0;
  
  int sentido = 4; //pin de control de motor PaP
  int clk = 7; //pin de clock para motor PaP
  int testled=13; //led onboard para pruebas
  int motor=9; //motor de carretel
  const int ledPin =  13;  
  
  int n=0; // variable para pruebas

  long int contador=0; //contador de pulsos para el feedrate
  int ref=0; //guardo el estado de la referenciada
  int fcarrera = 8; //pin del final de carrera
  int fcarreraest = 0; //variable donde guardo el estado del final de carrera

  int op=1; //opcion en el receving data
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

   op=1;
 
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
  


    
}


/*---------------------------------------*/
void encoder() //funcion que suma uno a <<pulsos>> con cada falling en el pin 2
   {    
    contador++ ;
    pul_encod++ ;
    if(contador==feedrate)
      {
        digitalWrite(clk, !digitalRead(clk));
        contador=0;
      }

    if(pul_encod==500) //acá hay que poner ca cantidad de pulsso que efectivamente de el encoder por revolucion
      rev++;
     
   }
  

//*************************/funcion serial event
void serialEvent() 
  {
  while (Serial.available())
      {
        rec = (long int)Serial.read();
        if(rec==2)
          referenciar();
        else{
            if(rec==q)
                {
                Serial.write(q);
                switch (op){
                       case 1:
                       feedrate=check;
                       break;
                       case 2:
                       rev_capa=check;
                       break;
                       case 3:
                       n_espiras=check;
                       break;
                        }
                  
                Serial.write(q);
                digitalWrite(testled, HIGH);
                op++; 
                rec=9;
                }
              else
                {
                  check=rec;
                  Serial.write(check);
                }
          }
      }    
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
            
      }
    ref=1;
    digitalWrite(sentido, HIGH);
  }
 




