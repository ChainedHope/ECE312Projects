#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 4567
#define BUF_SIZE 256
#define CLADDR_LEN 100

// See ChatClient.c
void error(char *msg) {
	perror(msg);
	exit(1);
}

//See ChatClient.c
void * receiveMessage(void * socket) {
	int sockfd, ret;
	char buffer[BUF_SIZE];
	sockfd = (int) socket;

	memset(buffer, 0, BUF_SIZE);

	while ((ret = read(sockfd, buffer, BUF_SIZE)) > 0) {
		printf("client: %s", buffer);
	}
	if (ret < 0)
		printf("Error receiving data!\n");
	else
		printf("Closing connection\n");
	close(sockfd);
}


//See ChatClient.c
void * sendMessage(void * socket) {
	char buffer[BUF_SIZE];
	int sockfd, set;
	sockfd = (int) socket;

	while(1){
		memset(buffer, 0, strlen(buffer); 
		//Seems to be a small bug if you go over the buffer limit or spam chat
		fgets(buffer, BUF_SIZE, stdin);
		if((write(sockfd, buffer, strlen(buffer))) == -1){
			printf("Error sending message\n");
			close(sockfd);
			exit(1);
		}
	}

	close(sockfd);

}
		       
		       
int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno, clilen;
	char buffer[BUF_SIZE];
	struct sockaddr_in serv_addr, cli_addr;
	pid_t childpid;
	char clientAddr[CLADDR_LEN];
	pthread_t rThread, sThread;
	int ret, set, n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	printf("Socket created...\n");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	printf("Binding done...\n");

	printf("Waiting for a connection...\n");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	
	//The while loop is checking to see if the socket is taking transimissions
	while (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) {
		inet_ntop(AF_INET, &(cli_addr.sin_addr), clientAddr, CLADDR_LEN);
		printf("Connection accepted from %s...\n", clientAddr);

		//creating a new thread for receiving messages from the client
		if (ret = pthread_create(&rThread, NULL, receiveMessage,
				(void *) newsockfd)) {
			printf("ERROR: Return Code from pthread_create() is %d\n", ret);
			error("ERROR creating thread");
		}

		//Creating a new thread for sending messages to the client
		if(set = pthread_create(&sThread, NULL, sendMessage, (void *)newsockfd)){
			printf("ERROR: Return Code from pthread_create() is %d\n", set);
			error("ERROR creating thread");

		}
	}
	
	//Prints an error if the connection was refused
	if (newsockfd < 0)
		error("ERROR on accept");
	close(sockfd);

	//Kill all the threads!
	pthread_exit(NULL);
	return 0;
}

