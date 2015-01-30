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
void error (char * mensaje)
{
	perror (mensaje);
	exit(1);
}

int main( int argc,char *argv[])		// ./servidor "puerto" "nombre del servidor"  
{
	if( argc!=3)
	{
		printf("\nEl numero de argumentos es incorrecto. %d\n", argc);
		exit(0);
	}

int ds; 				//descriptor del socket
struct sockaddr_in servidor, cliente;  //info. servidor y cliente 
int long_serv, long_cli, n , m ,p;	//LONGITUDES...
int serv_puerto;
char men_rec[MAXBUFF];  	//buffer de recepcion mensajes
char men_env[MAXBUFF];		//mensaje para enviar


serv_puerto=atoi(argv[1]);		//convierto de char a int	


//1 abrir socket (canal)

	if( (ds = socket(AF_INET, SOCK_DGRAM, 0) ) == -1)
	{
		error("socket");
		
	}

//2 asociar el canal y el puerto

	bzero( (char*) & servidor, sizeof (servidor));   //inicializo a cero la estructura
	servidor.sin_family = AF_INET; 			//familia	
	servidor.sin_addr.s_addr = INADDR_ANY; 		//cualquier IP*******************"nombre del servidor???????
	servidor.sin_port = htons (serv_puerto); /*formato red */
	


	long_serv = sizeof(servidor);
	
	if( bind(ds, (struct sockaddr *) &servidor, long_serv) == -1)
	{
		error("bind");
		
	}

//3 recibir datos del cliente:

	for(;;)
	{
		long_cli=sizeof(cliente);
		bzero( (char*) &cliente, long_cli); 	//inicializo a cero la estructura
		
		int is;
		fflush(stdin);
	
		for (is=0; is<MAXBUFF; is++)
		{	men_env[is]=0;
			
			men_rec[is]=0;
		}	

		//recibo el datagrama:
			n= recvfrom(ds, men_rec, MAXBUFF, 0, (struct sockaddr *) &cliente, &long_cli);
			if(n == -1)
			{ 
				error("recvfrom");
				
			}
		printf("\n Mensaje recibido: %s \n Longitud: %d .",men_rec,strlen(men_rec));

	
		//REALIZO EL SERVICIO:
			//invertir el mensaje:
				int i;	 	//indice de la cadena inversa
				int d=n-1;	//indice de la cadena original
				
				for( i=0; i<n; i++, d--)
				{
					men_env[i] = men_rec[d];
				}
		
		//4envio el mensaje al cliente:	
				m= sendto (ds, men_env, n, 0, (struct sockaddr *) &cliente, long_cli);
				if(m == -1)
				{
					error("sendto");
					
				}
				if(m != n)
				{
					printf("\nse han perdido caracteres\n");
					
				}
			

			printf("\n Mensaje enviado al cliente: %s \n",men_env);

	//envio el nombre del servidor pasado por la linea de ordenes

			p=strlen(argv[2]);
			n= sendto (ds, argv[2], p, 0, (struct sockaddr *) &cliente, long_cli);
				if(p == -1)
				{
					error("sendto");
					
				}
				if(n != p)
				{
					printf("\nse han perdido caracteres en el nombre del servidor\n");
					
				}
			

			printf("\n Mensaje enviado al cliente: %s \n",argv[2]);



	}		
		
close(ds); 	//5 cerrar el socket
}
		
		





