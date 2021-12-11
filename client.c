// Importa librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "utils.c"


// Constante con el número de puerto por el cual se va conectar el cliente
#define PORT 3490

// Constante con el número máximo de bytes que podemos envíar
#define MAXDATASIZE 300

void main(int argc, char *argv[]){


    // Declaración de variables y estructuras necesarias
    int sockfd, numbytes;             
    char * words[3];                // Para almacenar el comando dividido en 3
    char buf[MAXDATASIZE];          // Para almacenar el comando y respuesta del servidor
    struct hostent *he;             // Información del host
    struct sockaddr_in their_addr;  // Información de direcciones de conectores (puerto)


    // Verifica si se proporcionó un argumento de línea de comandos
    if(argc != 2) {
        fprintf(stderr, "Client-Usage: %s host_servidor\n", argv[0]);
        exit(1);
    }


    // Obtiene la información del host
    if((he = gethostbyname(argv[1])) == NULL) {

        // Si no obtiene información del host, termina el programa
        perror("gethostbyname()");
        exit(1);
    }else {

        // Si obtiene información, muestra el host
        printf("Client-The remote host is: %s\n", argv[1]);
    }


    // Crea un socket para envío y recepcion de info. con direcciones IPv4
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){

        // Si no se puede crear, muestra el error y termina el programa
        perror("socket()");
        exit(1);
    }else {

        // Si se crea, muestra un mensaje de éxito
        printf("Client-The socket() sockfd is OK...\n");
    }
        

    // Orden de bytes del host (4 bytes para direcciones IPv4 -> AF_INET)
    their_addr.sin_family = AF_INET;


    // Almacena en la estructura tipo sockaddr_in el puerto de comunicación 
    // y la dirección IP del host en el campo s_addr de la estructura interna sin_addr 
    printf("Server-Using %s and port %d...\n", argv[1], PORT);
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);


    // Establece en 0 el campo sin_zero de la estructura sockaddr_in
    memset(&(their_addr.sin_zero), '\0', 8);


    // Intenta hacer una conexión a un socket (descriptor de archivo 'sockfd')
    if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
        
        // Si la conexión falla, muestra el error y termina el programa 
        perror("connect()");
        exit(1);
    }
    else {

        // Si no falla, muestra un mensaje de éxito
        printf("Client-The connect() is OK...\n");
    }


    // Lee el mensaje del usuario
    printf("Enter a command: ");
    fgets(buf, MAXDATASIZE-1, stdin);

    // Divide el comando
    split_string(buf, words);

    // Valida el comando
    if (validate_command(words) == 1)
    {
        // Si es válido, envía mensaje al servidor
        if ((numbytes = send(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {

            // Si ocurre un error, se termina el programa
            perror("send()");
            exit(1);
        } else {

            // Si se envía con éxito, muestra un mensaje
            printf("Client-send() is OK...\n");
        }
    } else {

        // Si no es válido, muestra un mensaje de error y termina el programa
        printf("Invalid command: Please enter a valid command.\n");
        exit(1);
    }

    /* Recibe un mensaje del servidor; almacena el mensaje en el buffer (buf) y 
    el tamaño del mensaje escrito en el buffer es retornado (numbytes) */
    if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {

        // Si ocurre un error, se termina el programa
        perror("recv()");
        exit(1);
    } else {

        // Si se recibe el mensaje sin problema, muestra un mensaje de éxito
        printf("Client-The recv() is OK...\n");
    }


    // Al final del mensaje recibido coloca el caracter de fin d cadena (\0)
    //buf[numbytes] = '\0';


    // Muestra el mensaje recibido y cierra el socket (Descriptor de archivo)
    printf("Client-Received: %s\n", buf);
    printf("Client-Closing sockfd\n");
    close(sockfd);
}