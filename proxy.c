#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include<sys/socket.h>	

// Global variables
char url[1000];
char hostName[1000];
char path[1000];
char httpType[100];
char client_message[5000];
int socket_desc;
int client_sock;

//Function Declarations
void client_side  ();
void requestSubstring (char *origin, char **givenPointer);
void getRequest (char* line);
void getHostName ();


int main(int argc, char *argv[]){

    //checking to see if the number of arguments is acceptable
    if(argc != 2){
        printf("./proxy [PORT NUMBER]\n");
        return 1;
    }
    else if(isdigit(atoi(argv[1])) != 0){
        printf("Please enter a valid port number.");
        exit(0);
    }

// ------------------------- SERVER SIDE CODE -----------------------

	struct sockaddr_in server, client;

	int count;

    //getting the port number from the command line arguments
    char *c = argv[1];
    int port = atoi(c);

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1){
		printf("Could not create socket\n");
	}
	puts("Socket created\n");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	
	//Bind
    int bindStatus = bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
	if( bindStatus == -1){
		//print the error message
		perror("Binding failed");
		return 1;
	}
	printf("Binding done.\n");
	
	//Listen
	listen(socket_desc, 3);
    
	
	//Accept and incoming connection
	printf("Waiting for clients...\n");

    while(1){

		//accept connection from an incoming client
		client_sock = accept(socket_desc, NULL, NULL);
		if (client_sock < 0){
			perror("Connection failed");
			return 1;
		}
		printf("Connection accepted\n");

		//receiving data
        int recvStatus = recv(client_sock, client_message, 5000, 0);
	
        if (recvStatus == -1){
            printf("Error in receiving!");
            break;
        }

		//send the client message to getRequest which'll parse and return the URL
		getRequest(client_message);

		//send the URL to the client
		client_side();

    }

    close(socket_desc);
    return 0;
}





// ---------------------- EXTRACT URL FROM GET REQUEST ------------------------


void getRequest(char* line){

	//get length of line
    int len = strlen(line);
	int lastIndex;

	//initialize url_buffer to 0
	for(int i = 0; i < len; i++){
		url[i] = 0;
	}

	//loop starting from http:// all the way until before HTTP 1.1
    for(int i = 4; i < len; i++){
        if(line[i] == '\n'){
            break;
        }
		//add characters to buffer until we reach a space
		strncat(url, &line[i], 1);
    }
}




// -------------- CLIENT SIDE -----------------------

void client_side(){

	struct sockaddr_in server;
	struct hostent *hostAddr;

	int c = sizeof(server);
	int port = 80;
	int i;

	char *message;
	char ip[100];

	//retreive the host name
	getHostName();

	//getting the list of addresses
	hostAddr = gethostbyname(hostName);

	//checking if we successfully optain IP
	if(hostAddr == NULL){
		printf("Error");
		exit(-1);
	}
	else{
		printf("Got Host IP.\n");
	}

	//intialization
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	bcopy((char *) hostAddr->h_addr_list[0], (char *) &server.sin_addr.s_addr, hostAddr->h_length);

	//creating socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock < 0){
		printf("creating socket failed. Exiting.\n");
		exit(-1);
	}

	//checking if connection results in error
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
		puts("Connection Error.\n");
		exit(-1);
	}
	//otherwise it connected
	puts("Connected.\n");

	//creating the GET request
	char get[] = "GET ";
	char n_and_r[] = "\r\n\r\n";
	// char httpType[] = " HTTP/1.0\n";
	char end[100];
	int j;

	//creating a clone of the path 
	for(j = 0; path[j] != '\0'; j++){
		end[j] = path[j];
	}
	
	//concatenating a new string composed of GET (path name) (HTTP Type)
	strcat(get, url); //GET URL
	strcat(get, httpType); //GET (URL) + (Type)
	strcat(get, n_and_r); //GET (URL) + (Type) + (\r\n\r\n)

	message = get;

	// printf("Get Request: %s\n", message);

	char buffer[60000];
	int length = strlen(buffer);

	//setting buffer to 0
	for(int i = 0; i < length; i++){
		buffer[i] = 0;
	}

	while(1){

		//sending GET request
		int byte = send(sock, client_message, strlen(client_message), 0);
		int proxy_client = accept(client_sock, (struct sockaddr *)&server, (socklen_t*)&c);

		

		if (byte < 0) {
			puts("fail insending.");
		}
			//receiving content into buffer
			byte = recv(sock, buffer, sizeof(buffer), 0);
			if(byte <= 0){
				break;
			}
			//checking if send failed
			if(send(client_sock, buffer, byte, 0) < 0){
				puts("Send Failed.\n");
				return;
			}
			//sending the exact amount requested (this is to load JPEGs)
			send(sock, buffer, byte, 0);
			//resetting buffer
			memset(buffer, 0, 60000);
		}
	close(sock);
	return;
}


// -------------------- GET HOST NAME ----------------------

void getHostName(){

	//get length of url 
    int length = strlen(url);
	int lastIndex;

	//initialize array w/ 0's
	for(int i = 0; i < length; i++){
		hostName[i] = 0;
	}

	//intialize array w/ 0's
	for(int i = 0; i < length; i++){
		path[i] = 0;
	}

	//loop from after the http:// to the first / to get the host
    for(int i = 7; i < length; i++){
        if(url[i] == '/'){
			//store the last index to get the path
			lastIndex = i;
            break;
            }
        else{
            strncat(hostName, &url[i], 1);
        }
    }

	//loop from the last index of the host name to the end of the URL
	for(int i = lastIndex; i <length; i++){
		if(url[i] == ' '){
			break;
		}
		strncat(path, &url[i], 1);
	}
}


