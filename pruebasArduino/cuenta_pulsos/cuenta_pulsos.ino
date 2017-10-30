 
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

   

}

void loop() 
{
  // put your main code here, to run repeatedly:
    //Serial.println("inici");    
  // fcarreraest = digitalRead(fcarrera);
   // Serial.println(fcarreraest); //muestra el estado de fcarreraest. eliminar cuando todo funcione
    
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
    if(rec=='a')
      analogWrite(motor, 75);
    if(rec=='s')
      analogWrite(motor, 0);
    if(rec=='d')
      n=contador;
    if(rec=='e')
      Serial.println(n);
      
  }



/*---------------------------------------*/
void encoder() //funcion que suma uno a <<pulsos>> con cada falling en el pin 2
   {    
    contador++ ;
   }
  

