#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
	

void config_port_read(int fd);
char read_char(int fd);

int main(void){
        	
	int fd;/*File Descriptor*/

/*------------------------------- Opening the Serial Port -------------------------------*/

/* Change /dev/ttyACM0 to the one corresponding to your system */

   	fd = open("/dev/ttyACM0",O_RDWR | O_NOCTTY | O_NDELAY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
			   					/* O_RDWR   - Read/Write access to serial port       */
								/* O_NOCTTY - No terminal will control the process   */
								/* Open in blocking mode,read will wait              */
									
									                                        
									
   	if(fd == -1)						/* Error Checking */
   	   printf("  Error! in Opening ttyUSB0  \n");
   	else
   	   printf("\n  ttyUSB0 Opened Successfully ");

	
	config_port_read(fd);
			
	while(1){
		printf("%c", read_char(fd));
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

	tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */

	char var;   /* variable to store the data received              */
//	int  bytes_read = 0;    /* Number of bytes read by the read() system call */
//	int i = 0;

//	bytes_read = read(fd,&read_buffer,32); /* Read the data                   */
			
	read(fd,&var,1);

	return var;
}

























