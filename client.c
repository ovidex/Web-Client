#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include <string.h>
char* create_json_auth(char* user,char *pass){
	JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;
    json_object_set_string(root_object, "username", user);    
    json_object_set_string(root_object, "password", pass);    
    serialized_string = json_serialize_to_string_pretty(root_value);
   
    json_value_free(root_value);

	return serialized_string;
}
char* create_json_book(char* title,char *author,char *genre,int page_count,char* publisher){
	JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *serialized_string = NULL;
    json_object_set_string(root_object, "title", title);    
    json_object_set_string(root_object, "author", author);    
    json_object_set_string(root_object, "genre", genre);       
    json_object_set_number(root_object, "page_count", page_count);
    json_object_set_string(root_object, "publisher", publisher);     
    serialized_string = json_serialize_to_string_pretty(root_value);
   
    json_value_free(root_value);

	return serialized_string;
}
int main(int argc, char *argv[])
{
    char *message;
    char *response;
	char * cookie[1];
	char token[500]="NULL";
    int sockfd;
	int cookie_nr=0;
	while(1) {
	char* body_data[1];
	char user[100],pass[100],command[100];
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
	char* content_type = "application/json";
	scanf("%s",command);
	if(!strcmp(command,"exit")){
		close(sockfd);
		return 0;
	}
	else
	if(!strcmp(command,"register")){	
	printf("username=");	
	scanf("%s",user);
	printf("password=");
	scanf("%s",pass);
	body_data[0]=create_json_auth(user,pass);
	message = compute_post_request("3.8.116.10","/api/v1/tema/auth/register",content_type,body_data,1,NULL,0,NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
	 if(response!=NULL)
	 {if(basic_extract_json_response(response)!=NULL)puts(basic_extract_json_response(response));
	 else puts("Registration successful!");
	 puts(""); }
	}
	else
		if(!strcmp(command,"login")){
	printf("username=");	
	scanf("%s",user);
	printf("password=");
	scanf("%s",pass);
	body_data[0]=create_json_auth(user,pass);
	message = compute_post_request("3.8.116.10","/api/v1/tema/auth/login",content_type,body_data,1,NULL,0,NULL);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
	 if(response!=NULL){
		if(basic_extract_json_response(response)!=NULL)puts(basic_extract_json_response(response));
			else {
				puts("Authentification successful!");
			      char *cookie1 = strstr(response, "Cookie");
				  char cookie2[100];
					int poz = 0;
					for (int i = 0; ; ++i) 
						if(cookie1[i] == ';') 
						{
							poz = i;
							break;
						}
					strncpy(cookie2,cookie1,poz);					
					memset(cookie1, 0,sizeof(*cookie1));
					strcpy(cookie1,cookie2+8);
					cookie[0]=cookie1;
					cookie_nr++;
					//puts(cookie[0]);
			}
	 }
		}
	else
			if(!strcmp(command,"enter_library"))
			{
				message = compute_get_request("3.8.116.10", "/api/v1/tema/library/access", NULL, cookie, cookie_nr,NULL);
				send_to_server(sockfd, message);
				response = receive_from_server(sockfd);
				 if(response!=NULL)
				 {if(basic_extract_json_response(response)!=NULL)
					{	if(!strncmp(basic_extract_json_response(response),"{\"error\"",8))
						puts(basic_extract_json_response(response));
						else{
						strtok(basic_extract_json_response(response), "\" ");
						strtok(NULL,"\" ");
						strtok(NULL,"\" ");
						strcpy(token,strtok(NULL,"\" "));
						puts("You're in!");
						//printf("%s\n",token);
						}
					}
				 }
			}
	else if(!strcmp(command,"get_books"))
			{
				message = compute_get_request("3.8.116.10", "/api/v1/tema/library/books", NULL, cookie, cookie_nr,token);
				send_to_server(sockfd, message);
				response = receive_from_server(sockfd);
				 if(response!=NULL)
				 {if(strstr(response,"[")!=NULL)
					puts(strstr(response, "["));
				  else if(basic_extract_json_response(response)!=NULL){
					  puts(basic_extract_json_response(response));
				  }
				 }
						
			}
	else if(!strcmp(command,"get_book"))
	{	printf("id=");
		char id[100],aux[100]="/api/v1/tema/library/books/";
		scanf("%s",id);
		strcat(aux,id);
		message = compute_get_request("3.8.116.10", aux, NULL, cookie, cookie_nr,token);
		send_to_server(sockfd, message);
				response = receive_from_server(sockfd);
		if(response!=NULL)
		{if(basic_extract_json_response(response)!=NULL)
			puts(basic_extract_json_response(response));
		}
	}
	else if(!strcmp(command,"delete_book"))
	{
		printf("id=");
		char id[100],aux[100]="/api/v1/tema/library/books/";
		scanf("%s",id);
		strcat(aux,id);
		message = compute_delete_request("3.8.116.10", aux, NULL, cookie, cookie_nr,token);
		send_to_server(sockfd, message);
				response = receive_from_server(sockfd);
		if(response!=NULL)
		{if(basic_extract_json_response(response)!=NULL)
			puts(basic_extract_json_response(response));
		}
	}
	else if(!strcmp(command,"add_book"))
	{char title[200],author[200],genre[200],publisher[200];
	 int page_count;
	printf("title=");	
	scanf("%s",title);
	printf("author=");
	scanf("%s",author);
	printf("genre=");
	scanf("%s",genre);
	printf("publisher=");
	scanf("%s",publisher);
	printf("page_count=");
	scanf("%d",&page_count);
	char* bod=create_json_book(title,author,genre, page_count, publisher);
	strcpy(*body_data,bod);
	
		message = compute_post_request("3.8.116.10","/api/v1/tema/library/books",content_type,body_data,1,NULL,0,token);
				send_to_server(sockfd, message);
				response = receive_from_server(sockfd);
				if(response!=NULL)
		{if(basic_extract_json_response(response)!=NULL)
			puts(basic_extract_json_response(response));
		}
				
	}
	else if(!strcmp(command,"logout"))
	{message = compute_get_request("3.8.116.10", "/api/v1/tema/auth/logout", NULL, cookie, cookie_nr,NULL);
				send_to_server(sockfd, message);
				response = receive_from_server(sockfd);
				if(response!=NULL)
				 {for (int i=0; i<cookie_nr; i++)
					 {cookie[i][0] = 0;} 
				if(basic_extract_json_response(response)!=NULL)
			puts(basic_extract_json_response(response));
				else
					puts("Logged out");}
					
	}
	else puts("Invalid command");
	}

    return 0;
}
