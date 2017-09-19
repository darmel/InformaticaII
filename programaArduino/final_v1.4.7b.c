/* 
 * File:   newmain_v0.c
 * Author: Chirino Juan
 *
 * Created on 14 de septiembre de 2015, 09:41
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include <math.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 4000000

#define CW PORTCbits.RC1
#define Clock PORTCbits.RC3
#define PWM PORTCbits.RC2
#define OPTO PORTBbits.RB1


void interrupt vector();

/*      CALCULOS
 *
 * TRAFO 125;APILACION 32 -> C=43mm
 * ALAMBRE DIAMETRO 0.5
 *
 * Reduccion motor=7.407
 * ENCODER=240P/R
 *
 * Pulsos del encoder por espira= 1777
 *
 * espitas por capa=86
 *
 * pulsos del encoder por capa=1777*86= 152822
 *
 * pasos Stepper por capa= 43mm/0.07mm= 614
 *
 * Pulsos Encoder por Paso Stepper= 152822/614= ***248****
 *
 * Carga TMR1=2^16-248= 65288 -> FF08h
 *
 * TMR1H=FF
 * TMR1L=08
 */

/*---------------ESTRUCTURA TECLADO----------------
 *      COLUMNAS
 *        0       1       2       3       4
 *  F 0 [0x00]  [0x20]  [0x40]  [0x60]  [0x80]
 *  I
 *  L 1 [0x08]  [0x28]  [0x48]  [0x68]  [0x88]
 *  A
 *  S 2 [0x10]  [0x30]  [0x50]  [0x70]  [0x90]
 *
 *    3 [0x18]  [0x38]  [0x58]  [0x78]  [0x98]
 */



#define DATA_BUS PORTD
void inc_display();
void set_inst();
void limpiar_display();
void limpiar_caracter(char Y, char X, char N_caracteres);    //borra la cantidad de catacteres pedida
void set_cursor(char Y,char X);
void escribe_caracter(char Ch);
const char Adress[2][16]={{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F},
                          {0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F}};

//----------------------------Funciones teclado-------
char numero_teclado();
float recupera_dato();
void recarga();

//--------------------------cadenas de texto display------------------
const char Test[13]={"Test Display"};

const char Text_prog_0[]={"Programa"};
const char Text_prog_1[]={"select: <      >"};
const char Text_prog_opc1[]={"Manual"};
const char Text_prog_opc2[]={"Autom."};

const char Text_prog_manual_0[]={"Prog. Manual 1/1"};
const char Text_prog_manual_1_1[]={"RPM motor:<    >"};

const char Text_prog_auto_0[]={"Prog. Autom. 1/4"};
char Text_prog_auto_1_2[]={"N espiras:      "};
const char Text_prog_auto_1_3[]={"logn.[mm]:      "};
char Text_prog_auto_1_4[]={"  alam.[mm]:      "};

const char Text_bobinando_0[]={"Bobinando: OK"};
const char Text_bobinando_1[]={"Esp:    /    "};



char opcion_programa;   //1->Auto 0-> Manual
char i;

//------------------bariables carha numerica del teclado------------
char carga_entera=1,cargando_numero;    //banderas
char valor_entero[4],N_cifras_E=0;
char valor_decimal[2],N_cifras_D=0;

//------------------Bariables bobinando------------------
char cuenta_espiras[4];


void referenciado();        //funcion para referenciar el carro al momento del encendido de la maquina
void calculo();


//----------------------variables usuario------------
short int num_espiras;      //variable para el numero de espitas del trafo
char long_carretel;          //largo del carretel en milimetros
float diametro_alambre;      //diametro del alambre en milimetros


//--------------------variables de calculo-----

short int pulsos_capa;
short int pasos_capa;

long int pulsos;            //cantidad de pulsos (PULLING)
short int pasos;            //cantidad de pàsos (pulling)
short int espiras=0;        //cantidad de espìras(pulling)

char alto, bajo;            //variables carga del TMR1
unsigned short int aux_1,aux_2;     //variable auxiliar

void main(){

    PWM=0;
    espiras=0;
    
    
    INTCON=0xC0;        //habilitar interrupcion por TMR1
    PIE1=0x01;
    PIR1=0;

    T1CON=0x03;         //seteo de TMR1
    CCP2CON=0x00;

    TRISC=0x01;         //RC0->encoder-A motor
    TRISB=0xFF;     //RB<7:3> = MM74C922 DATA_LINES ;RB<0>= MM74C922 Data avalible
    TRISD=0x00;     //display LCD data bus DB<0:7>
    TRISE=0x08;     //RE0=RS; RE1=R/W; RE2=E

    ANSEL=0x00;     //PORTE -> Dijital I/O
    ANSELH=0x00;    //PORTB -> Digital I/O
    OPTION_REG=0xC0;//RBPU= enable
    WPUB=0x02;      //RB<7:4>=Pull-up enable
    IOCB=0x00;      //RB<7:4>=interrupt enable

    Text_prog_auto_1_4[0]=0xC9;
    Text_prog_auto_1_2[1]=0xD2;

    inc_display();

    DATA_BUS=0x38;     //funcion set: 1 lineas; caracteres 5x8
    PORTEbits.RE0=0;//RS=0 (IR)
    set_inst();

    DATA_BUS=0x06;     //entry mode set: I/D=1; S=0
    PORTEbits.RE0=0;//RS=0 (IR)
    set_inst();


    DATA_BUS=0x0E;     //display=on ; cursor=on; parpadeo=off
    PORTEbits.RE0=0;//RS=0 (IR)
    set_inst();

    DATA_BUS=0x01;     //clear display
    PORTEbits.RE0=0;//RS=0 (IR)
    set_inst();

    //--------------SELECCION DE PROGRAMA----------------

    selecion_de_programa:

    set_cursor(0,0);
    for(i=0;Text_prog_0[i]!='\0';i++){
        escribe_caracter(Text_prog_0[i]);
    }

    set_cursor(1,0);
    for(i=0;Text_prog_1[i]!='\0';i++){
        escribe_caracter(Text_prog_1[i]);
    }

    opcion_programa=2;
    set_cursor(1,9);
    for(i=0;Text_prog_opc2[i]!='\0';i++){
        escribe_caracter(Text_prog_opc2[i]);
    }

    cargando_numero=1;
    while(cargando_numero==1){  //lectura de teclado
        while(PORTBbits.RB0==0){}
        switch(PORTB&0xF8){
            case 0x68:      //tecla UP
                if(opcion_programa==1){     //opcion Automatica
                    opcion_programa=2;
                    set_cursor(1,9);
                    for(i=0;Text_prog_opc2[i]!='\0';i++){
                        escribe_caracter(Text_prog_opc2[i]);
                    }
                }
                else{                   //opcion manual
                    opcion_programa=1;
                    set_cursor(1,9);
                    for(i=0;Text_prog_opc1[i]!='\0';i++){
                        escribe_caracter(Text_prog_opc1[i]);
                    }
                }
            break;

            case 0x70:      //tecla DOWN
                if(opcion_programa==1){ //opcion Automatica
                    opcion_programa=2;
                    set_cursor(1,9);
                    for(i=0;Text_prog_opc2[i]!='\0';i++){
                        escribe_caracter(Text_prog_opc2[i]);
                    }
                }
                else{                   //opcion manual
                    opcion_programa=1;
                    set_cursor(1,9);
                    for(i=0;Text_prog_opc1[i]!='\0';i++){
                        escribe_caracter(Text_prog_opc1[i]);
                    }
                }
            break;

            case 0x58:      //ENTER
                cargando_numero=0;
            break;

        }
        while(PORTBbits.RB0==1){}
    }

    if(opcion_programa==1){}//PROGRAMA MANUAL

    else{               //PROGRAMA AUTOMATICO


    set_cursor(0,0);                                    //Prog. Autom. 1/1
    for(i=0;Text_prog_auto_0[i]!='\0';i++){
        escribe_caracter(Text_prog_auto_0[i]);
    }



    //----------PROGRAMA AUTOMATICO CARGA 2/4 (Numero de espiras)---------------
    set_cursor(0,13);
    escribe_caracter('2');

    set_cursor(1,0);                                    //Prog. Autom.
    for(i=0;Text_prog_auto_1_2[i]!='\0';i++){
        escribe_caracter(Text_prog_auto_1_2[i]);
    }

    cargando_numero=1;
    set_cursor(1,12);
    recarga();

    while(cargando_numero==1){

        while(PORTBbits.RB0==0){}
        __delay_ms(10);

        switch(PORTB&0xF8){
            case 0x58:      //ENTER
                cargando_numero=0;
            break;

            case 0x60:      //suprimir
                limpiar_caracter(1,12,4);      //2°renglon del caracter 12 al
                recarga();
            break;

            default:
                if(numero_teclado()!=0xFF ){
                    if(N_cifras_E!=4){
                        escribe_caracter(numero_teclado()|0x30);//escribir en mumero de la tecla presionada
                        valor_entero[N_cifras_E]=numero_teclado();
                        N_cifras_E++;
                    }
                }
            break;
        }

        while(PORTBbits.RB0==1){}
    }

    long_carretel=recupera_dato();


    //---------PROGRAMA AUTOMATICO CARGA 3/4 (Lingitud del carretel)--------------
    set_cursor(0,13);
    escribe_caracter('3');

    set_cursor(1,0);
    for(i=0;Text_prog_auto_1_3[i]!='\0';i++){
        escribe_caracter(Text_prog_auto_1_3[i]);
    }

    cargando_numero=1;
    set_cursor(1,12);
    recarga();

    while(cargando_numero==1){

        while(PORTBbits.RB0==0){}
        __delay_ms(10);

        switch(PORTB&0xF8){
            case 0x58:      //ENTER
                cargando_numero=0;
            break;

            case 0x60:      //suprimir
                limpiar_caracter(1,12,4);      //2°renglon del caracter 12 al
                recarga();
            break;

            default:
                if(numero_teclado()!=0xFF ){
                    if(N_cifras_E!=3){
                        escribe_caracter(numero_teclado()|0x30);//escribir en mumero de la tecla presionada
                        valor_entero[N_cifras_E]=numero_teclado();
                        N_cifras_E++;
                    }
                }
            break;
        }

        while(PORTBbits.RB0==1){}
    }

    diametro_alambre=recupera_dato();

    //---------PROGRAMA AUTOMATICO CARGA 4/4 (Diemetro de alambre)--------------
    set_cursor(0,13);
    escribe_caracter('4');

    set_cursor(1,0);
    for(i=0;Text_prog_auto_1_4[i]!='\0';i++){
        escribe_caracter(Text_prog_auto_1_4[i]);
    }

    cargando_numero=1;
    set_cursor(1,12);
    recarga();

    while(cargando_numero==1){

        while(PORTBbits.RB0==0){}
        __delay_ms(10);

        switch(PORTB&0xF8){
            case 0x18:
                if(carga_entera==1){escribe_caracter('.');}
                carga_entera=0;
            break;

            case 0x58:      //ENTER
                cargando_numero=0;
            break;

            case 0x60:      //suprimir
                limpiar_caracter(1,12,4);      //2°renglon del caracter 12 al
                recarga();
            break;

            default:
                if(numero_teclado()!=0xFF ){
                    if(((N_cifras_E!=1)&(carga_entera==1))|((N_cifras_D!=2)&(carga_entera==0))){
                    escribe_caracter(numero_teclado()|0x30);//escribir en mumero de la tecla presionada
                    }
                    if((carga_entera==1)&(N_cifras_E!=1)){
                        valor_entero[N_cifras_E]=numero_teclado();
                        N_cifras_E++;
                    }
                    else{
                        if((N_cifras_D!=2)&(carga_entera==0)){
                        valor_decimal[N_cifras_D]=numero_teclado();
                        N_cifras_D++;
                        }
                    }
                }
            break;
        }

        while(PORTBbits.RB0==1){}
    }

    diametro_alambre=recupera_dato();

    recarga();

    }

    referenciado();     //poner el carro en la posicion 0

    calculo();

    CW=1;

    while((PORTB&0xF8)!=0x98){}

    limpiar_display();
    
    set_cursor(0,0);
    for(i=0;Text_bobinando_0[i]!='\0';i++){
        escribe_caracter(Text_bobinando_0[i]);
    }

    set_cursor(1,0);
    for(i=0;Text_bobinando_0[i]!='\0';i++){
        escribe_caracter(Text_bobinando_1[i]);
    }

    PWM=1;
    
    for(i=0;i<4;i++){cuenta_espiras[i]='0';}

    while(num_espiras!=espiras){
    //while(1){
        if(pulsos>=1777){            
            cuenta_espiras[0]++;
            set_cursor(1,7);
            escribe_caracter(cuenta_espiras[0]);
            if(cuenta_espiras[0]==':'){
                cuenta_espiras[0]='0';
                cuenta_espiras[1]++;
                set_cursor(1,6);
                for(i=0;i<2;i++){escribe_caracter(cuenta_espiras[i]);}
                if(cuenta_espiras[1]==':'){
                    cuenta_espiras[1]='0';
                    cuenta_espiras[2]++;
                    set_cursor(1,5);
                    for(i=0;i<3;i++){escribe_caracter(cuenta_espiras[i]);}
                    if(cuenta_espiras[2]==':'){
                        cuenta_espiras[2]='0';
                        cuenta_espiras[3]++;
                        set_cursor(1,6);
                        for(i=0;i<4;i++){escribe_caracter(cuenta_espiras[i]);}
                    }
                }
            }
            

            pulsos=pulsos-1777;
            espiras++;
        }
    }

    INTCON=0x00;
    PWM=0;
    while(1){}
}

void referenciado(){
    CW=0;
    while(OPTO==0){
        Clock=0;
        __delay_us(10);
        Clock=1;
        __delay_ms(5);
    }
}

void calculo(){
    pulsos_capa=round((long_carretel/diametro_alambre)*1777);
    pasos_capa=round(long_carretel/0.071);

    aux_1=round(pulsos_capa/pasos_capa);
    aux_2=65536-aux_1;

    alto=aux_2>>8;
    bajo=aux_2-(alto<<8);

    TMR1H=alto;
    TMR1L=bajo;

}

void interrupt vector(){
   //TMR1H=0xFF;
   //TMR1L=0xFE;

    TMR1H=alto;
    TMR1L=bajo;

    Clock=0;
    __delay_us(10);
    Clock=1;

    pasos++;
    pulsos=pulsos+aux_1;

    if(pasos==pasos_capa){
        CW=~CW;
        pasos=0;
    }

    TMR1IF=0;
}

char numero_teclado(){
    char numero;
    switch (PORTB&0xF8){
        case 0x00:                  //COLUMNA 0
        numero=1;
        break;

        case 0x08:
        numero=4;
        break;

        case 0x10:
        numero=7;
        break;

        case 0x20:                  //COLUMNA 1
        numero=2;
        break;

        case 0x28:
        numero=5;
        break;

        case 0x30:
        numero=8;
        break;

        case 0x38:
        numero=0;
        break;

        case 0x40:                  //COLUMNA 2
        numero=3;
        break;

        case 0x48:
        numero=6;
        break;

        case 0x50:
        numero=9;
        break;

        default:
        numero=0xFF;
        break;
    }

    return numero;
}

float recupera_dato(){
    float dato;
    char exponente;
    exponente=N_cifras_E-1;
    dato=0;
    switch(exponente){
        case 0:
            dato=valor_entero[0];
        break;

        case 1:
            dato=valor_entero[0]*10;
            dato=dato+valor_entero[1];
        break;

        case 2:
            dato=valor_entero[0]*100;
            dato=dato+valor_entero[1]*10;
            dato=dato+valor_entero[2];
        break;

        case 3:
            dato=valor_entero[0]*1000;
            dato=dato+valor_entero[1]*100;
            dato=dato+valor_entero[2]*10;
            dato=dato+valor_entero[3];
        break;
    }
    dato=dato+valor_decimal[0]*0.1;
    dato=dato+valor_decimal[1]*0.01;

    return dato;
}

void recarga(){
    carga_entera=1;
    N_cifras_E=0;
    N_cifras_D=0;
    for(i=0;i<4;i++){valor_entero[i]=0;}
    for(i=0;i<2;i++){valor_decimal[i]=0;}
}

void limpiar_caracter(char Y, char X, char N_caracteres){
    set_cursor(Y,X);

    for(i=0;i<N_caracteres;i++){
        escribe_caracter(' ');
    }

    set_cursor(Y,X);
}
//----------------------FUNCIONES PARA EL LCD--------------------
void inc_display(){
    __delay_us(15000);      //esperar a que se alimente LCD

    DATA_BUS=0x30;
    PORTE=0x00;         //RS=0; R/W=0; (escribir IR)
    PORTEbits.RE2=1;    //habilitar display
    __delay_us(4100);       //delay 4.1mS
    PORTEbits.RE2=0;    //deshabilitar diaplay

    DATA_BUS=0x30;
    PORTE=0x00;         //RS=0; R/W=0; (escribir IR)
    PORTEbits.RE2=1;    //habilitar display
    __delay_us(100);        //delay 100uS
    PORTEbits.RE2=0;    //deshabilitar diaplay

    DATA_BUS=0x30;
    PORTE=0x00;         //RS=0; R/W=0; (escribir IR)
    PORTEbits.RE2=1;    //habilitar display
    __delay_us(100);          //delay 1uS
    PORTEbits.RE2=0;    //deshabilitar diaplay
}

void set_inst(){
    PORTEbits.RE2=1;    //habilitar LCD
    __delay_us(2000);        //delay 2mS
    PORTEbits.RE2=0;    //deshabilitar LCD
}


void set_cursor(char Y,char X){
    DATA_BUS=0x80|Adress[Y][X];    //posicionar cursor
    PORTEbits.RE0=0;//RS=0 (IR)
    set_inst();
}

void limpiar_display(){
    DATA_BUS=0x01;     //clear display
    PORTEbits.RE0=0;//RS=0 (IR)
    set_inst();
}
void escribe_caracter(char Ch){
    DATA_BUS=Ch;
    PORTEbits.RE0=1;    //RS=1 (DR)
    set_inst();
}