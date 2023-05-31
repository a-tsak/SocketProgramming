/* Anastasia Tsakalou
2022201900226
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void write_file(int sockfd, char *buffer, char *receiver, char *sender);

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, port_number;
	socklen_t client_size;
	char buffer[256];
	struct sockaddr_in server_address, client_address;
	int n;
	
	if (argc<2){
		printf("\nYou have to provide a port number.\n\n");
		exit(1);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		printf("\nThere is an error with creating the socket.\n\n");
		exit(1);
	}
	
	bzero((char *) &server_address, sizeof(server_address));
	port_number = atoi(argv[1]);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port_number);
	
	if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address))<0){
		printf("\nThere is an error with binding.\n\n");
		exit(1);
	}
	
	listen(sockfd,5);
	
	client_size = sizeof(client_address);
	newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &client_size);
	
	if (newsockfd<0){
		printf("\nThere is an error with accepting the connection with the socket.\n\n");
		exit(1);
	}
	
	FILE *fp;
	char response[50], receiver[100], sender[100], str[256], str2[506], file_name[100];
	char *token;
	const char s[2] = "\n";
	char *p;
	
	int i=0, x=0;
	
	while(1)		// Tha kleisei otan kleisei kai o client
	{
		i++;
        
		n = read(newsockfd,buffer,255);
		
		strncpy(response,buffer,2);	// Einai 1 xaraktiras to response alla thelei +1 megethos gia to strncpy()
		
		if (response[strlen(response)-1]=='\n'){
			response[strlen(response)-1]='\0';
		}
		
		if (strncmp(response,"1",1)==0){
				
				n = read(newsockfd,buffer,255);
				
				// ----------- Gia ton sender -----------
				
				if (i>1){
					if (token!=NULL){
						token = strtok(buffer, s);
					}
				}
				else{
					token = strtok(buffer, s);
				}
				strncpy(sender,token,strlen(token)+1);
				
				
				// ----------- Gia ton receiver -----------
				
				if (token!=NULL){
					token = strtok(NULL, s);
				}
				strncpy(receiver,token,strlen(token)+1);
				
				// ----------- Gia to message -----------
				
				if (token!=NULL){
					token = strtok(NULL, s);
				}
				strncpy(buffer,token,strlen(token)+1);
				
				
				
				if (n < 0) 
					printf("\nThere is an error reading from the socket.\n\n");
				else{
					write_file(newsockfd,buffer,receiver,sender);
				}
				
				bzero(receiver,100);		// O sender kai to response den xreiazontai na katharistoun, o sender den allazei oso trexei to programma
				bzero(buffer,256);
				
				
				
				
				
		}
		else if (strncmp(response,"2",1)==0){
			
			x++;
			// Apo to socket: Den xreiazetai na diavasei to onoma tou sender, afou den allazei
			// Den xreiazetai na diavasei to message
			// Den xreiazetai na diavasei to response ksana (to exei idi diavasei)
			// Den xreiazetai na diavasei to onoma tou receiver
			
			// Apo to arxeio: Diavazei to sender|receiver|message, kai to stelnei ston client

			
			if (sender==NULL || strlen(sender)==0){	// An h prwti epilogi einai to 2 (akoma ki an exei ksana kanei login o user)
				n = read(newsockfd,buffer,255);
			}
			
			if (buffer!=NULL){
				token = strtok(buffer,s);
				strncpy(sender,token,strlen(token)+1);
				
			}
			
			
			
			file_name[0] = '\0';
			char ext[] = ".txt";
			
			if (sender[strlen(sender)-1] == '\n'){
				sender[strlen(sender)-1] = '\0';
			}
			strncpy(file_name,sender,strlen(sender));		// Vale to onoma tou receiver sto filename
			file_name[strlen(file_name)]='\0';
			
			if (x==1){
				strncat(file_name, ext, 5);			// 5 giati strlen(".txt") = 4 chars + 1 char to '\0' = 5
			}
			
			fp = fopen(file_name,"r");
			
			if (fp==NULL){
				printf("\nThere is an error opening the file!\n\n");
			}
			
            while(fgets(str2, sizeof(str2), fp) != NULL) {
				n = write(newsockfd,str2,strlen(str2));
			}
            
			
			
			if (n < 0) {
            	printf("\nThere was an error writing to socket\n");
        	}
        	
        	bzero(receiver,100);
			bzero(buffer,256);
			
			fclose(fp);
			
		}
		else if(strncmp(response,"3",1)==0){
			exit(1);
		}
		
   		
	}
	close(newsockfd);
	close(sockfd);
	return 0; 
}

void write_file(int sockfd, char *buffer, char *receiver, char *sender){
	
	FILE *fp;
	char ext[] = ".txt";
	char filename[100];
	
	if (filename!=NULL){
		
		filename[strlen(filename)] = '\0';
		
		filename[0] = '\0';
		
	}
	
	if (receiver[strlen(receiver)-1] == '\n'){
		receiver[strlen(receiver)-1] = '\0';
	}
	
	receiver[strlen(receiver)] = '\0';
	
	strncpy(filename,receiver,strlen(receiver)+1);		// Vale to onoma tou receiver sto filename
	filename[strlen(filename)]='\0';
	strncat(filename, ext, 5);			// 5 giati strlen(".txt") = 4 chars + 1 char to '\0' = 5
	
	
	if (filename!=NULL){
		
		if (filename[strlen(filename)-1] == '\n'){
			filename[strlen(filename)-1] = '\0';
		}
	}
	
	if (filename!=NULL){
		
		if (sender[strlen(sender)-1] == '\n'){
			sender[strlen(sender)-1] = '\0';
		}
	}
	
	
	fp = fopen(filename, "a");
	if (fp==NULL){
		printf("\nThere is an error with opening the file.\n\n");
	}

	fprintf(fp,"%s|", sender);		// Gia to format tou arxeiou, stin ekfwnisi zitaei na exei kai ta 2 username kai to swma tou minimatos to arxeio
	fprintf(fp,"%s|", receiver);
	fprintf(fp, "%s\n", buffer);
	bzero(buffer, 256);
	
	fclose(fp);
	
	return;
}

