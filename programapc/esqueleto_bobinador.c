#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pulsos_rev 1000
#define length_step 0.01

void referenciar(void);	//referenciado de carro
 
int calc_feedrate(float diam);	//calculo de relacion de vueltas y avance del carro
int calc_retorno(float diam, float lon);		//numero de pulsos a los que el carro cambia de direccion
int calc_N_pulsos(int N_esp);		//calculo de valores

void bobinar(int feedrate, int retorno, int N_pulsos);

int send_data(int feedrate,int retorno,int N_pulsos);
char read_port();

int main(){

	//variables de control de flujo de programa
	int MENU=0;
	char opc;
	int rep;
	int pepe=1;	//repeticion del menu principal
	
	//banderas
	int ref_done=0;		//bandera de referenciado de carro
	int datos_done=0;	//bandera de carga de datos
	
	//Variables de carga de datos
	float diam;	//diametro del alambre
	int N_esp;	//numero de espiras
	float lon;	//longitud del carretel
	
	//variables para transmitir a la bobinadora
	int feedrate;	//relacion de vueltas y avance del carro
	int retorno;	//numero de pulsos a los que el carro cambia de direccion
	int N_pulsos;	//numero de pulsos a los que la maquina se detiene
	
	
	while(pepe){
		
		switch(MENU){
		
			case 0:
				printf("		MENU\n\n");
				printf("1-Cargar datos\n");
				printf("2-Referenciar carro\n");
				printf("3-Bobinar\n");
				printf("4-Salir\n");
				scanf("%d", &MENU);
				break;

			case 1:
				printf("\n	CARGA DE DATOS\n\n");
				printf("-Diametro del alambre[mm]: ");
				scanf("%f", &diam);
				printf("-Longitud del carrerel[mm]: ");
				scanf("%f", &lon);
				printf("-Numero de espiras: ");
				scanf("%d", &N_esp);
				
				feedrate = calc_feedrate(diam);	//relacion de vueltas y avance del carro
				retorno = calc_retorno(diam,lon);		//numero de pulsos a los que el carro cambia de direccion
				N_pulsos = calc_N_pulsos(N_esp);	//calculo de valores
					
				//printf("%d %d %d \n\n", feedrate, retorno, N_pulsos);	
			
				datos_done=1;
				MENU=0;
				break;
				
			case 2:
				if(ref_done==0){
					referenciar();
					ref_done=1;
				}
					printf("Carro referenciado\n");
					
				MENU=0;
				break;

			case 3:
				if(datos_done==0){	//verificar que haya datos
					printf("ERROR!, datos no cargados\n\n");
				}
				else{ 
					if(ref_done==0){//verificar que la maquina este referenciada
						printf("ERROR!, maquina no referenciada\n\n");
					}
					else{ // ya se puede bobinar
						printf("Enebre el alambre en el porta alambre\n");
						do{
							rep=0;
							printf("\nLa maquina comienzara a bobinar, ¿Desea continuar?(S/n)"); scanf(" %c", &opc);
							if((opc!='S') && (opc!='s') && (opc!='N') && (opc!='n')){
								rep=1;
								printf("Opcion no valida");
							}
						}while(rep==1);
						
						if(opc=='S'||opc=='s'){
							bobinar(feedrate, retorno, N_pulsos);
							ref_done=0; //despues del bobnado la maquina fuera de su origen 	
						}
					}
				}
	
				MENU=0;
				break;
			
			case 4:
				pepe=0;
				break;
			
			default:
				printf("ERROR!, opcion no valida\n\n");
				MENU=0;
		}
	}
	return 0;
}

int calc_feedrate(float diam){	//calculo de relacion de vueltas y avance del carro
	return round((length_step*pulsos_rev)/diam);
}

int calc_retorno(float diam, float lon){//numero de pulsos a los que el carro cambia de direccion
	return round(pulsos_rev*(lon/diam));
}

int calc_N_pulsos(int N_esp){		//calculo de valores
	return N_esp*pulsos_rev;
}

void bobinar(feedrate, retorno, N_pulsos){

	char dato_retornado;
	
	printf("Transmitiendo datos ...\n");
	
	if(send_data(feedrate, retorno, N_pulsos)==0){ //cheking de la transmision
		printf("Transmision completa\n");
		
		printf("Bobinando..............  0 %c \n",'%');
		
		for(int i=0; i<20; i++){
			dato_retornado=read_port();
			switch(dato_retornado){
				case 'A': // bobinando +5%
					printf("Bobinando..............%3d %c \n",5*(i+1),'%');
					break;
					
				//aca poner el resto de los casos 
			
			}
		}
	}
	else{
		printf("ERROR de transmision\n");
	}
}

int send_data(int feedrate,int retorno,int N_pulsos){

	return 0;//transmitio todo piola 
}

char read_port(){

	return 'A';	//bobinando
}

void referenciar(void){
	//aca hay que mandarle un codigo de referenciado al arduino
}





















