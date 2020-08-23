#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h> //hostent
 
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"



//char* findIP(char *hostname);

int main(int argc , char *argv[])
{
    struct sockaddr_in server;
    int socket_desc;
    char server_reply[1000];
    //char* unip[200];

    //char *hostname = "www.google.com";
    //unip[100]=findIP(hostname);

    socket_desc = socket(AF_INET , SOCK_STREAM , 0); //SOCK_STREAM (oriented TCP protocol) ------SOCK_DGRAM which indicates the UDP protocol    
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    int port=55555;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("%sConnect error%s\n",ANSI_COLOR_RED, ANSI_COLOR_RESET);
        return 1;
    }
    else
    {
        printf("%sConnected %s\n",ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        char auth[]="987654321";
        if( send(socket_desc , auth , strlen(auth) , 0) < 0)
        {
            printf("%s\n Error", ANSI_COLOR_RED);
            return 1;
        }
        if ((recv(socket_desc , server_reply , 2000 , 0)) && (strcmp("Error auth",server_reply))!=0 )
        {
            printf("%s%s%s\n",ANSI_COLOR_BLUE, server_reply, ANSI_COLOR_RESET);
        }
    } 
    
    while(1)
    {    
        char message[]="Online";
        if( send(socket_desc , message , strlen(message) , 0) < 0)
        {
            printf("%sSend failed\n", ANSI_COLOR_RED);
            return 1;
        }
        //Receive a reply from the server
        if( recv(socket_desc , server_reply , 2000 , 0) < 0)
        {
            printf("%sReceived failed\n", ANSI_COLOR_RED);
        }
         
        printf("%sServer reply:  ", ANSI_COLOR_GREEN);
        printf("%s%s\n%s", ANSI_COLOR_YELLOW, server_reply, ANSI_COLOR_RESET);
        if(strcmp(server_reply,"PressKey")==0)
        {
            system("osascript -e 'tell application \"System Events\" to key code '");
        }
        sleep(1);
    }
    return 0;
}

/*char* findIP(char *hostname)
{
    
    char ip[100];
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
         
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        //gethostbyname failed
        herror("gethostbyname");
    }
     
    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
    }
     
    printf("%s resolved to : %s \r\n" , hostname , ip);
    return ip;
}*/




