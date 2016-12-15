#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#define PORT 4567
#define BUF_SIZE 256
#define CLADDR_LEN 100


/*
*  error - prints the error message and exits the program
*/
void error(char *msg) {
	perror(msg);
	exit(1);
}

/*
* Recieves messages from the socket.
*/
void * receiveMessage(void * socket) {
	int sockfd, ret;
	char buffer[BUF_SIZE];
	sockfd = (int) socket;

	memset(buffer, 0, strlen(buffer));

	while ((ret = read(sockfd, buffer, BUF_SIZE)) > 0) {
		printf("server: %s", buffer);
	}
	if (ret < 0)
		printf("Error receiving data!\n");
	else
		printf("Closing connection\n");
	close(sockfd);
}

/*
* Sends Messages to the socket for transmission
*/
void * sendMessage(void * socket) {
	char buffer[BUF_SIZE];
	int sockfd, set;
	sockfd = (int) socket;

	while(1){
		memset(buffer, 0, BUF_SIZE);
		fgets(buffer, BUF_SIZE, stdin);
		if((write(sockfd, buffer, strlen(buffer))) == -1){
			printf("Error sending message\n");
			close(sockfd);
			exit(1);
		}
	}

	close(sockfd);

}

/*
* Main Function
*/
int main(int argc, char *argv[]) {
	//Variable setup
	int sockfd, portno, n;  
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[BUF_SIZE];
	pthread_t rThread, sThread;
	int ret, set;
	
	//Checks for command-line inputs
	if(argc < 2){
		printf(stderr, "Usage %s hostname\n", argv[0]);
		exit(0);
	}
	
	portno=PORT; // Sets the port to the constant
	sockfd= socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("Error: Failed to Open Socket");
	server=gethostbyname(argv[1]);
	if(server==NULL){
		error("Error: Host does not exist");
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port=htons(portno);
	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
		error("Error: Could not connect");
	}
	
	//This while loop doesnt actually have to be here..... 
	//Originally it was used to check connections but that caused some bugs
	//Now the threads check for a connection in their while loops
	while(1){
		//creating a new thread for receiving messages from the client
		if (ret = pthread_create(&rThread, NULL, receiveMessage,
				(void *) sockfd)) {
			printf("ERROR: Return Code from pthread_create() is %d\n", ret);
			error("ERROR creating thread");
		}

		//Creating a new thread for sending messages to the client
		if(set = pthread_create(&sThread, NULL, sendMessage, (void *)sockfd)){
			printf("ERROR: Return Code from pthread_create() is %d\n", set);
			error("ERROR creating thread");

		}
		break;
	}

	//Kill the threads!
	pthread_exit(NULL);
	return 0;
}

