/* Anastasia Tsakalou
2022201900226
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int main(int argc, char *argv[])
{
    int sockfd, port_number, n;			// sockfd = sock file descriptor, port_number = dinetai ws argument stin entoli ektelesis tou server, n = timi epistrofis twn read,write
    struct sockaddr_in server_address;
    struct hostent *server;
    char buffer[256], response[50], receiver[100], sender[100];		// buffer = message, response = choice (1,2,3)		//, username[100];
    
    if (argc < 3) {
       printf("\nYou have to provide an IP address.\n\n");
	   exit(1);
    }
    
    port_number = atoi(argv[2]);		// To port_number na ginei apo string pou dinetai, se int
    sockfd = socket(AF_INET, SOCK_STREAM, 0);		// Dimiourgia socket
    if (sockfd < 0){
        printf("\nThere is an error with creating the socket.\n\n");
        exit(1);
	}
	
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("\nThere is an error with finding the host.\n\n");
        exit(1);
    }
    
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);	// Metatrepei to port_number apo host byte order se network byte order
    if (connect(sockfd,(struct sockaddr *) &server_address,sizeof(server_address)) < 0){
        printf("\nThere is an error with connecting to the socket.\n\n");
	}
    
    int i=0;		// Gia na min uparxei apeiros arithmos
    int x = 0;
    
	printf("\nWelcome! What is your username?\n\n");
    fgets(sender,99,stdin);
    
	while(i<100)	// Estw oti diarkei gia 100 prospatheies
    {
    	i++;
        
		printf("\nDo you want to:\n(1) Send a new message?\n(2) Read your message(s)?\n(3) Log out?\n\nType (1), (2), or (3)\n\n");
		
		fgets(response,49, stdin);
		if (response[strlen(response)-1]=='\n'){
			response[strlen(response)-1]='\0';
		}
		
		n = write(sockfd,response,255);
		if (n < 0){
        	printf("\nThere is an error writing RESPONSE to socket\n\n");	//)rror("ERROR writing to socket");
    	}
        
		if (strncmp(response,"1",1)==0){		// Send a new message (1)
        	
        	x++;	//
        	
        	printf("\nReceiver: ");
			fgets(receiver, 99, stdin);
			receiver[strlen(receiver)] = '\0';
			
			printf("\nMessage: ");
			fgets(buffer,255,stdin);
			buffer[strlen(buffer)] = '\0';
			
			
            n = write(sockfd,sender,strlen(sender));	// Stelnw ton sender sto socket tou server
            
			if (n < 0){
            	printf("\nThere is an error writing to socket\n\n");
        	}
            
            
			n = write(sockfd,receiver,strlen(receiver));	// Stelnw ton receiver sto socket tou server
            
			if (n < 0){
            	printf("\nThere is an error writing to socket\n\n");
        	}	
            

			n = write(sockfd,buffer,strlen(buffer));		// Stelnw to minima sto socket tou server
			
			if (n < 0){
            	printf("\nThere is an error writing to socket\n\n");
        	}
            
        	bzero(receiver,100);	// Gia tin apostoli minimatos (epilogi 1), allazei to minima (buffer), kai o paraliptis kathe fora, o sender kai h epilogi oxi
			bzero(buffer,256);		// Opote katharismos twn strings sta sockets
			
		}
		
		else if (strncmp(response,"2",1)==0){	// Read messages (2)
			
			x++;
			
			bzero(receiver,100);	// Gia tin lipsi minimatwn (epilogi 2), allazei to minima (buffer), kai o paraliptis kathe fora, o sender kai h epilogi oxi
			bzero(buffer,256);		// Opote katharismos twn strings sta sockets
			
			if (x==1){		// An h prwti epilogi einai to 2, tote stelnei to onoma tou sender gia na to exei sigoura o server
				n = write(sockfd,sender,strlen(sender));
			}
			
			n = read(sockfd,buffer,255);	// Diavazei apo ton server auto pou o server diavase apo to arxeio tou receiver.txt, ta minimata tou xristi
			if (n < 0){
            	printf("\nThere is an error with reading the socket.\n\n");
            }
            
			printf("\nMessages:\n");		// Deixnei ta minimata pou exoun stalthei ston xristi pou xrisimopoiei ton client
			if (buffer!=NULL){
				printf("%s\n", buffer);
			}
			else{
				printf("Your email inbox is empty!\n");
			}
			
		}
		
		else if (strncmp(response,"3",1)==0){	// Log out (3)
			printf("\nYou successfully logged out of your email account.\n\n");
			exit(1);
		}
		
		else{
			printf("\nThere was an error. Please try again, type (1), (2), or (3).\n\n");
		}
		
		
		
    }
    
    close(sockfd);
    
    return 0;
}
