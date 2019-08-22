#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
void* clientThread(void* client_socket)
{
    char buffer[256] = "sucessfully connected to server\n";
    /*send*/
    send(*(int*)client_socket, buffer, sizeof(buffer), 0);
    int flag = 1;
    while(flag)
    {
        /*receive command*/
        int reval;
        reval=recv(*(int*)client_socket, buffer, sizeof(buffer), 0);
        if(reval < 0)
        {
            perror("message getting error.");
            flag = 0;
        }

        if(strcmp(buffer, "get")==0)
        {
            printf("got get command\n");
            /*receive group name from client*/
            reval=recv(*(int*)client_socket, buffer, sizeof(buffer), 0);
            if(reval < 0)
            {
                perror("message getting error.");
                flag = 0;
            }
            printf("group to get: %s\n",buffer);
            /*send retrieved message to client*/
            strcpy(buffer, "message retrieved\n");
            send(*(int*)client_socket, buffer, sizeof(buffer), 0);
        }
        else if(strcmp(buffer, "post")==0)
        {
            printf("got post command\n");
            /*receive group name from client*/
            reval=recv(*(int*)client_socket, buffer, sizeof(buffer), 0);
            if(reval < 0)
            {
                perror("message getting error.");
                flag = 0;
            }
            printf("group to post: %s\n",buffer);
            /*receive message client wants to post*/
            reval=recv(*(int*)client_socket, buffer, sizeof(buffer), 0);
            if(reval < 0)
            {
                perror("message getting error.");
                flag = 0;
            }
            printf("message to post: %s\n",buffer);
            /*send post confirmation*/
            strcpy(buffer, "post completed\n");
            send(*(int*)client_socket, buffer, sizeof(buffer), 0);
        }
        else
        {
            printf("got end command\n");
            /*receive group name from client*/
            reval=recv(*(int*)client_socket, buffer, sizeof(buffer), 0);
            if(reval < 0)
            {
                perror("message getting error.");
                flag = 0;
            }
            flag = 0;
        }
    }
    printf("exit this thread\n");
    pthread_exit(0);
}
int main(int argc, char** argv)
{   
	/*variables*/
	int sock;
	struct sockaddr_in serv_addr;

	/*create*/
	sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock<0)
	{
		perror("create failed.");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port=htons(8000);
	
	/*bind*/
	if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
	{
		perror("bind failed");
		exit(1);
	}
	
	/*listen*/
	listen(sock, 10);

	/*accept*/
	do
	{   
        int client_socket;
        client_socket = accept(sock, NULL, NULL);
		if(client_socket < 0)
		{
			perror("accept failed.");
		}
		else
		{
			pthread_t tid;
            
			pthread_create(&tid, NULL, clientThread, &client_socket);
		}
	}
	while(1);
    close(socket);
	return 0;
}