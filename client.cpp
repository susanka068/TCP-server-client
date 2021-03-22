// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <bits/stdc++.h>



#define COLOR_NONE "\033[m"
#define COLOR_RED "\033[1;37;41m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_CYAN "\033[0;36m"
#define COLOR_GREEN "\033[0;32;32m"
#define COLOR_GRAY "\033[1;30m"
#define COLOR_PURPLE "\033[0;35m"
#define COLOR_BLUE "\033[0;34m"

using namespace std ; 

char* mode = "guest" ; 

void welcome_manager(){
	cout<<"Welcome aboard , manager\n\n " ; 

}

void sendsync(int& sock , string& msg){
	send(sock , msg.data() , msg.size() , 0 ) ;
	sleep(2) ;  
}


bool autheticate_manager(int& sock ){
			string username , password ;  
			cout<<"username: " ; 
			cin>>username ;
			cout<<endl ;  
			cout<<"password: " ; 
			cin>>password ; 
			cout<<endl ; 
			send(sock , username.data() , username.size() , 0 ) ; 
			sleep(2) ;

			send(sock , password.data() , password.size() , 0 ) ;
			//sleep(2) ;

			char authentication_buffer[1024]  = {0}  ;

			recv( sock , authentication_buffer , 1024 , 0 ) ;
			//sleep(2);

			string authentication_string  = authentication_buffer ; 

			//printf("%s\n", authentication_string );

			cout<<authentication_string<<endl ;

			if(authentication_string == "authenticated" )
			{
				mode = "manager" ; 
				printf( COLOR_GREEN "manager authentication successful \n\n") ;
				return true ; 
				
			}
			else if( authentication_string == "unauthenticated" ){
				printf( COLOR_RED "wrong username of password \n\n") ;
			}
			else{
				cout<<"having issue while authenticating \n";
			}
			return false ; 
		
	}


	void authentication_prompt(int& sock){

	printf("welcome aboard\n would you like to get authenticated as a manager ? [y/n] " );
	string prompt_response ; 
	cin>>prompt_response ; 
	sendsync(sock , prompt_response) ; 
	if(prompt_response=="y")
		autheticate_manager(sock) ; 
}

int main(int argc, char *argv[]) 
{ 

	int PORT = 8888 ; 

	if(argc> 2 )
		PORT = atoi(argv[2]) ; 

	string client_id ;

	if(argc>1)
		client_id = argv[1] ; 

	char* serv_address = "127.0.0.1" ;
	if(argc > 3)
		serv_address = argv[3] ; 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	string hello = "Hello from client"; 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, serv_address, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

	authentication_prompt(sock) ;

	string query_string = "" , key= "" , value=""  ; 
	char response_buffer[1024] = { 0 } ; 
	int _id ; 

	while(true){
		printf( COLOR_BLUE "%s-%s@" COLOR_CYAN "query-shell> " COLOR_NONE , mode , argv[1]) ; 
		cin>>query_string ; 
		string query_info ;
		if(query_string =="put")
		{
			cin>>key>>value ; 
			query_info = query_string + "#" + key + "#" + value + "#" + client_id + "#" ; 
		}
		else if(query_string == "get")
		{
			cin>>key ; 
			query_info = query_string + "#" + key + "#" + value + "#" + client_id + "#" ; 
		}
		else if(query_string == "putm")
		{
			cin>>key>>value>>_id ; 
			query_info = query_string + "#" + key + "#" + value + "#" + client_id + "#" + to_string( _id) + "#" ; 
		}
		else if(query_string == "getm")
		{
			cin>>key>>_id ; 
			query_info = query_string + "#" + key + "#" + value + "#" + client_id + "#" + to_string( _id) + "#" ; 
		}

		


		send(sock , query_info.c_str() , query_info.size()  , 0) ; 


		if(query_string == "exit")
		{
			break ; 

		}

		bzero(response_buffer , 1024 ); 

		recv(sock , response_buffer , 1024 , 0 ) ; 

		printf( COLOR_YELLOW "%s\n", response_buffer );
		query_string = "" ; key ="" ; value ="" ; query_info="" ; 
	}
	
	send(sock , hello.data() , hello.size() , 0 ); 
	printf("Hello message sent\n"); 
	valread = read( sock , buffer, 1024); 
	printf("%s\n",buffer ); 
	return 0; 
} 