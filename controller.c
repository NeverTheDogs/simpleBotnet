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
    system("clear");
	struct sockaddr_in server;
    int socket_desc;
    //char* unip[200];

    //char *hostname = "www.google.com";
    //unip[100]=findIP(hostname);
    char *message = malloc(1000 * sizeof(char));
    char server_reply[1000];

    //char *message[1000];// server_reply[1000];
    //char name[1000]="666";
    socket_desc = socket(AF_INET , SOCK_STREAM , 0); //SOCK_STREAM (oriented TCP protocol) ------SOCK_DGRAM which indicates the UDP protocol    
    if (socket_desc == -1)
    {
        printf("%sCould not create socket", ANSI_COLOR_RED);
    }
    int port=55555;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	//Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("\n%sError connect!!\n\n",ANSI_COLOR_RED );
        return 1;
    }
    printf("%sConnected!!!\n", ANSI_COLOR_GREEN);
    sleep(1);
    printf("Try auth...%s\n", ANSI_COLOR_RESET);
    sleep(2);
    /*if( send(socket_desc , name , strlen(name) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }*/
    char auth[]="123456789";
    if( send(socket_desc , auth , strlen(auth) , 0) < 0)
    {
        printf("%s\n Error send auth", ANSI_COLOR_RED);
        return 1;
    }
    if ((recv(socket_desc , server_reply , 2000 , 0)) && (strcmp("Error auth",server_reply))!=0 )
    {
        printf("%s%s%s\n ",ANSI_COLOR_BLUE, server_reply, ANSI_COLOR_RESET);
        while(1)
        {   
            printf("\n%s1)%s Press key \n", ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET);
            printf("%s2)%s Uccidi \n", ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET);
            printf("%s3)%s Mordi \n", ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET);
            printf("%s4)%s Brucia \n\n", ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET);
            printf("%sFai la tua scelta:%s ", ANSI_COLOR_GREEN, ANSI_COLOR_BLUE );
            scanf("%s" , message);
            //Send some data
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
            if ((strcmp("Client offline",server_reply))==0)
            {
                printf("%sServer reply:  ", ANSI_COLOR_GREEN);
                printf("%s%s\n", ANSI_COLOR_RED, server_reply);
                exit(0);
            }
            printf("%sServer reply:  ", ANSI_COLOR_GREEN);
            printf("%s%s\n", ANSI_COLOR_YELLOW, server_reply);
        }
    }
    else
    {
        printf("%s%s\n",ANSI_COLOR_RED, server_reply);
        return 1;
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




