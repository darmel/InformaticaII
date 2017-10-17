#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
	

void config_port(int fd);
char read_char(int fd);
int read_int(int fd);
char caracter;
void config_port_read(int fd); //lee configuracioes del puerto

void write_port(char dato,int fd);
void write_port_int(int numero, int fd);

int l; //para contar las veces que se lee
int entero;
int n_enviado=56;
int n_rec=0;

int feedrate=0;
int rev_capa=0;
int n_espiras=0;

int main(void){
    
    int op=0;    	
	int fd;/*File Descriptor*/

/*------------------------------- Opening the Serial Port -------------------------------*/

/* Change /dev/ttyACM0 to the one corresponding to your system */

   	fd = open("/dev/ttyACM0",O_RDWR | O_NOCTTY );	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
			   					/* O_RDWR   - Read/Write access to serial port       */
								/* O_NOCTTY - No terminal will control the process   */
								/* Open in blocking mode,read will wait              */
									
									                                        
									
   	if(fd == -1)						/* Error Checking */
   	   printf("  Error! in Opening ttyUSB0  \n");
   	else
		{
			printf("\n +-------------------------------------+\n");
			printf(" | Puerto ttyACM0 Abierto Exitosamente |\n");
			printf(" +-------------------------------------+ \n");
			printf("int %d \n", sizeof(int));
			printf("long int %d \n", sizeof(long int));
			printf("unsigned int %d \n", sizeof(unsigned int));
			printf("double %d \n", sizeof(double));
		}
		
	config_port_read(fd);
	
	
	getchar();
	system("clear"); //clears the screen
	
	
	printf("\n +-------------------------------------+");
	printf("\n |------Proyecto Informatica 2---------| \n");
	printf(" +-------------------------------------+\n");	
	printf("\n \ningreso de datos\n");
	 
		
	while(op!=3)
		{
		printf("\n1- Ingresar feedrate \n2- ingresar vueltas por capa \n3- total de vueltas\n");
		
		scanf("%d", &feedrate );
		scanf("%d", &rev_capa );
		scanf("%d", &n_espiras);
		op=1;
		switch(op)
			{
				case 1:
						l=0;
						n_enviado=feedrate;
						while(n_enviado!=1) //nviamos hasta que nos dan un eco correcto
								{
								write_port_int(n_enviado,fd);
								n_rec = read_int(fd);
								printf("%d\n", n_rec); //muestra los ecos erroneos
								l++;
								}
						write_port_int(1,fd);
						printf("numero recibido %d\n veces %d", n_rec, l);
						op=2; //sigo con rev_capa
						break;
						
			}		
	}			
	close(fd); /* Close the serial port */

	return 0;
}

void config_port_read(int fd){

/*---------- Setting the Attributes of the serial port using termios structure --------- */
		
	struct termios SerialPortSettings;	/* Create the structure                          */

	tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

	/* Setting the Baud rate */
	cfsetispeed(&SerialPortSettings,B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings,B9600); /* Set Write Speed as 9600                       */

	/* 8N1 Mode */
	SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
		
	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
		
		
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
		
	/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] = 1; /* Read at least 1 characters */
	SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */


	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
	    printf("  ERROR ! in Setting attributes\n");
	else
        printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");
}

char read_char(int fd){
	        /*------------------------------- Read data from serial port -----------------------------*/
	//static int cont=0;
	tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */

	char var;   /* variable to store the data received              */
//	int  bytes_read = 0;    /* Number of bytes read by the read() system call */
//	int i = 0;

//	bytes_read = read(fd,&read_buffer,32); /* Read the data                   */
			
//	printf("%d ", cont++);
	read(fd,&var,1);

	return var;
}

 int read_int(int fd){
	        /*------------------------------- Read data from serial port -----------------------------*/
	static int cont=0;
	tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */

	int var;   /* variable to store the data received              */
//	int  bytes_read = 0;    /* Number of bytes read by the read() system call */
//	int i = 0;

//	bytes_read = read(fd,&read_buffer,32); /* Read the data                   */
			
	//printf(" antes del read %d \n", var);
	read(fd,&var,2);
	
	return var;
}
void write_port(char dato, int fd){

/*------------------------------- Write data to serial port -----------------------------*/

	int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */ 

	bytes_written = write(fd,&dato,sizeof(dato));/* use write() to send data to port*/
									     /* "fd" - file descriptor pointing to the opened serial port */
									     /*	"dato" - address of the buffer containing data    */
									     /* "sizeof(write_buffer)" - No of bytes to write             */	
	//printf("\n  %c written to ttyUSB0",dato);
	//printf("\n  %d Bytes written to ttyUSB0", bytes_written);
	//printf("\n +----------------------------------+\n\n");
}

void write_port_int(int numero, int fd){

/*------------------------------- Write data to serial port -----------------------------*/

	int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */ 

	bytes_written = write(fd,&numero,sizeof(numero));/* use write() to send data to port*/
									     /* "fd" - file descriptor pointing to the opened serial port */
									     /*	"dato" - address of the buffer containing data    */
									     /* "sizeof(write_buffer)" - No of bytes to write             */	
	//printf("\n  %c written to ttyUSB0",numero);
	//printf("\n  %d Bytes written to ttyUSB0", bytes_written);
	//printf("\n +----------------------------------+\n\n");
}

