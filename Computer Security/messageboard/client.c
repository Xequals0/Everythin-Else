#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
int main(int argc, char** argv)
{	
	/*variables*/
	int sock;
	struct sockaddr_in serv_addr;
	struct hostent *server;
    
	/*create*/
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		fprintf(stderr, "failed to create socket.\n");
		return -1;
	}
	serv_addr.sin_family = AF_INET;
    server = gethostbyname("localhost");
    if(server == 0)
    {
        perror("get host name failed\n");
        exit(0);
    }
    memcpy(&serv_addr.sin_addr, server->h_addr, server->h_length);
	serv_addr.sin_port=htons(8000);
	
	/*connect*/
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		fprintf(stderr, "failed to connect.\n");
		close(sock);
		return -1;
	}
    /*receive*/
    char buffer[256];
    int reval;
    reval=recv(sock, buffer, sizeof(buffer), 0);
    if(reval < 0)
    {
        perror("message getting error.");
        return -1;
    }
    else
    {
        printf("%s\n", buffer);
    }
    int flag = 1;
    while(flag)
    {
        memset(buffer,0,strlen(buffer));
        printf("please enter a command:");
        gets(buffer);
        if(strcmp(buffer, "get")==0)
        {
            /*send get command to server*/
            send(sock, buffer, sizeof(buffer), 0);
            
            printf("please enter a group name which you want to retrieve messages from:");
            memset(buffer,0,strlen(buffer));
            gets(buffer);
            /*send group name to server*/
            send(sock, buffer, sizeof(buffer), 0);
            /*get retrived message*/
            memset(buffer,0,strlen(buffer));
            reval=recv(sock, buffer, sizeof(buffer), 0);
            if(reval < 0)
            {
                perror("message getting error.");
                return -1;
            }
            printf("%s\n", buffer);
        }
        else if(strcmp(buffer, "post")==0)
        {
            /*send post command to server*/
            send(sock, buffer, sizeof(buffer), 0);
            printf("please enter a group name which you want to post message to:");
            memset(buffer,0,strlen(buffer));
            gets(buffer);
            /*send group name to server*/
            send(sock, buffer, sizeof(buffer), 0);
            /*send message client want to post to server*/
            printf("please enter message you want to post:\n");
            memset(buffer,0,strlen(buffer));
            gets(buffer);
            send(sock, buffer, sizeof(buffer), 0);
            /*receive post confirmation from server*/
            memset(buffer,0,strlen(buffer));
            reval=recv(sock, buffer, sizeof(buffer), 0);
            if(reval < 0)
            {
                perror("message getting error.");
                return -1;
            }
            printf("%s\n", buffer);
        }
        else if(strcmp(buffer, "end")==0)
        {
            send(sock, buffer, sizeof(buffer), 0);
            flag = 0;
        }
        else
        {
            printf("%s\n", buffer);
            printf("please enter get, post or end as a command.\n");
        }
        
    }
    close(sock);
    return 0;
}