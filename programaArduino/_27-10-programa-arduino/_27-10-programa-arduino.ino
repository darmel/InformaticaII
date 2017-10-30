 
  long int contador=0; //contador de pulsos para el feedrate
  long int pul_encod=0; //contador de pulsos del encoder para las revolucion completa
  long int rev=0; //variable para guardar las revoluciones/espiras por capa
  long int rev_t=0; //variable para contar las revoluciones total
  long int pulsos_rev; //cantidad de pulsos por revolucion. Definido

  
  
  long int rev_capa=5; // revoluciones por capa
  long int n_espiras=25; //cantidad total de espiras
  long int feedrate=10; //inicializado para referenciar

  long int rec=0; //variable para guardar lo que recibo
  long int check=0;
  
  int sentido = 4; //pin de control de motor PaP
  int clk = 7; //pin de clock para motor PaP
  int testled=13; //led onboard para pruebas
  int motor=9; //motor de carretel
  const int ledPin =  13;  
  
  int n=0; // variable para pruebas

  
  int ref=0; //guardo el estado de la referenciada
  int fcarrera = 8; //pin del final de carrera
  int fcarreraest = 0; //variable donde guardo el estado del final de carrera

  int op=1; //opcion en el receving data
  int q=1; //variable para enviar en el check, para enviar intiguer y no char
  int vel=250; //variable de velocidad
  
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

    if(ref==0)
      {
        referenciar();      
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

    if(pul_encod>=500){ //acá hay que poner ca cantidad de pulsso que efectivamente de el encoder por revolucion
      rev++;
      rev_t++;
      pul_encod=0;
      Serial.write("espira n:"); Serial.println(rev_t);
    }
        
    if(rev>=rev_capa){
      digitalWrite(sentido, !digitalRead(sentido));
      rev=0;
        }
      
    if(rev_t>=n_espiras)
      analogWrite(motor, 0);
     
   }
  

//*************************/funcion serial event
void serialEvent() 
  {
  while (Serial.available())
      {
        rec = (long int)Serial.read();
        if(rec==2)
          referenciar();
          
        if(rec==3)
          analogWrite(motor, vel);
          
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
    fcarreraest = digitalRead(fcarrera); 
    digitalWrite(sentido, LOW);
    while (fcarreraest==HIGH)
      {
        digitalWrite(clk, !digitalRead(clk));
        delay(50);
        fcarreraest = digitalRead(fcarrera);    
      }
      
    ref=1;
    digitalWrite(sentido, HIGH);
    analogWrite(motor, vel);
  }
 




