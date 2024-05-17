#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}

// argc = total number of parametres (file_name and Port number)
// argv = file_name and Port number
int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "Port number isn't provided.");
		exit(1);
	}

	int sockfd, newsockfd, portno, check;
	char buffer[255];

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t cli_len;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) error("Error opening Socket.");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("Binding failed");
	
	listen(sockfd, 5);
	cli_len = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
	if(newsockfd < 0) error("Error on Accept.");

	while(1){
		bzero(buffer, 255);
		
		check = read(newsockfd, buffer, 255);
		if(check < 0) error("Error on reading.");
		printf("%s\n", buffer);
		
		bzero(buffer, 255);

		fgets(buffer, 255, stdin);

		check = write(newsockfd, buffer, strlen(buffer));
		if(check < 0) error("Error on writing.");

		int i = strncmp("bye", buffer, 3);
		if(i == 0) break;
	}

	close(newsockfd);
	close(sockfd);
	return 0;
}
