/* 
 * Author: David Vela Tirado
 * Software Developer
 * email:  david.vela.tirado@gmail.com
 * 
 *  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>


#define MAXBUFF 100 //longitud máxima mensajes replicar con ECO

#define PORTCLIE 5001  //puerto del cliente


void error (char * mensaje)
{
	perror (mensaje);
	exit(1);
}


int main()		// ./cliente  
{

int ds; 				//descriptor del socket
struct sockaddr_in servidor, cliente;  //info. servidor y cliente 
int long_serv, long_cli, n , m ;	//LONGITUDES...
int serv_puerto;
char men_rec[MAXBUFF];  	//buffer de recepcion mensajes
char men_env[MAXBUFF];		//mensaje para enviar
char men_rec2[MAXBUFF];		//mensaje para recibir nombre del servidor
char localhost[MAXBUFF]= "127.0.0.1";

//1 abrir socket (canal)

	if( (ds = socket(AF_INET, SOCK_DGRAM, 0) ) == -1)
	{
		error("socket");
		
	}

//2 asociar el canal y el puerto

	bzero( (char*) & cliente, sizeof (cliente));   //inicializo a cero la estructura
	cliente.sin_family = AF_INET; 			//familia	
	cliente.sin_port = htons (PORTCLIE); /*formato red */
	
	cliente.sin_addr.s_addr = inet_addr(localhost); 		// IP de localhost

	long_cli = sizeof(cliente);
	
	/*if( bind(ds, (struct sockaddr *) &cliente, long_cli) == -1)
	{
		error("bind");
		
	}*/
		
		bzero( (char*) & servidor, sizeof (servidor));   //inicializo a cero la estructura
		//servidor.sin_family = AF_INET; 			//familia
		
		//servidor.sin_port = htons (PORTSERV); 	/*formato red */
		
		//servidor.sin_addr.s_addr = INADDR_ANY; 		//cualquier IP*******************"nombre del servidor???????

		//long_serv = sizeof(servidor); 	


	for(;;)
	{
		//REALIZO LA LECTURA DE LOS DATOS DEL TECLADO:
		int i;
		fflush(stdin);
		
		for (i=0; i<MAXBUFF; i++)
		{	men_env[i]=0;
			men_rec[i]=0;
			men_rec2[i]=0;
		}		
			
		
		printf("\n ESCRIBIR CADENA: ");		
		if ( (fgets(men_env, MAXBUFF, stdin) == NULL) && (n=strlen(men_env)==0) ) //COMPRUEBO: si es esta vacio me salgo
			{
				error("fgets");
				
			}
		if( (n=strlen(men_env)) == 1)
			{
				printf("\nCadena vacia.\n ADIOS.\n");
				break;
			}


	
		
		//3envio el mensaje al servidor:
			
		
		m= sendto (ds, men_env, n, 0, (struct sockaddr *) &cliente, long_cli);
				if(m == -1)
				{
					error("sendto");
					
				}

					if(m != n)
				{
					printf("\nse han perdido caracteres\n");
					
				}
			
				printf("\nLe he enviado esta cadena al servidor: %s\n",men_env);
		
		
		//4recibir datagrama:
			n= recvfrom(ds, men_rec, MAXBUFF, 0, (struct sockaddr *) &servidor, &long_serv);
			if(n == -1)
			{ 
				error("recvfrom");
				
			}
			

		n= recvfrom(ds, men_rec2, MAXBUFF, 0, (struct sockaddr *) &servidor, &long_serv);
			if(n == -1)
			{ 
				error("recvfrom2");
				
			}
			
	
		//VISUALIZO LA CADENA RECIBIDA.
			printf("\n NOMBRE DEL SERVIDOR: %s",men_rec2 );
			printf("\n CADENA RECIBIDA: %s", men_rec);
			printf("\n\n\n");
		
	}		
		
close(ds); 	//5 cerrar el socket
}
		
		
