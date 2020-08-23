#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h> //for threading , link with lpthread

void *controller_handler(void *);
void *client_handler(void *);

int client_on=0;
int mex1=0;

int main(int argc , char *argv[])
{
	struct sockaddr_in server, controller;
    int socket_server, new_socket, c, *new_sock; 

    socket_server = socket(AF_INET , SOCK_STREAM , 0); //SOCK_STREAM (oriented TCP protocol) ------SOCK_DGRAM which indicates the UDP protocol    
    if (socket_server == -1)
    {
        printf("Could not create socket");
    }
    int port=55555;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( port );
	//Bind
    if( bind(socket_server,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("Bind Error");
    }
    else
    {
        printf("Bind on port %d\n",port);
    }     
    //Listen
    listen(socket_server , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_server, (struct sockaddr *)&controller, (socklen_t*)&c)) )
    {    
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;

        char message[200];

        memset(message,0x00,sizeof(message));
        recv(new_socket, message, 2000, 0);
        if (strcmp("123456789", message) == 0)
        {

                write(new_socket , "Hello Mr. Blue" , 100);
                if( pthread_create( &sniffer_thread , NULL ,  controller_handler, (void*) new_sock) < 0)
                {
                    perror("could not create thread");
                    return 1;
                }
                //char *controller_ip = inet_ntoa(controller.sin_addr);
                //int controller_port = ntohs(controller.sin_port);
                puts("Connection controller accepted...");    
                //Reply to the client
                //char *mex = "Hello Client";
                //write(new_socket , mex , strlen(mex));
                
                //Now join the thread , so that we dont terminate before the thread
                //pthread_join( sniffer_thread , NULL);            
        }
        else if (strcmp("987654321", message) == 0)
        {
            client_on=1;
            write(new_socket , "Hello Mr. Cazzi" , 100);
            if( pthread_create( &sniffer_thread , NULL ,  client_handler, (void*) new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
            //char *controller_ip = inet_ntoa(controller.sin_addr);
            //int controller_port = ntohs(controller.sin_port);
            puts("Connection client accepted...");
        }
        else
        {
             write(new_socket , "Error auth" , 100);
        }
        
    }
    //close(socket_server);
}

void *controller_handler(void *socket_server)
{
    //Get the socket descriptor
    int sock = *(int*)socket_server;
    int read_size;
    char controller_message[200];
    //Receive a message from client
    while( (read_size = recv(sock , controller_message , 2000 , 0)) > 0 )
    {
        if (client_on!=0)
        {
            int res = atoi(controller_message);
            puts(controller_message);
            memset(controller_message, 0, sizeof(controller_message));
            switch(res)
            {
                case 1:
                    //Send the message back to client
                    write(sock , "Premo freccia su" , 100);
                    mex1=1;
                    break;
                case 2:
                    write(sock , "uccido per te" , 100);
                    mex1=2;
                    break;
                case 3:
                    write(sock , "mordo per te" , 100);
                    break;
                case 4:
                    write(sock , "brucio per te" , 100);
                    break;
                default :
                    write(sock , "Messaggio errato" , 100);
                    break;
            }
        }
        else
        {
            write(sock , "Client offline" , 100);
        }
    }
    if(read_size == 0)
    {
        puts("Controller disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }    
    //Free the socket pointer
    free(socket_server); 
    return 0;
}



void *client_handler(void *socket_server)
{
    //Get the socket descriptor
    int sock = *(int*)socket_server;
    int read_size;
    char client_message[200];
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        puts(client_message);
        if (strcmp("Online", client_message) == 0)
        {
            if (mex1==1)
            {
                mex1=0;
                write(sock , "PressKey" , 100);
            }
            else if (mex1==2)
            {
                mex1=0;
                write(sock , "porcamadonna" , 100);
            }
            else
            {
                write(sock , "OK" , 100);
                mex1=0;
            }
        }
    }
    if(read_size == 0)
    {
        client_on=0;
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    //Free the socket pointer
    free(socket_server); 
    return 0;
}







