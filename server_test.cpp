#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <thread> 

using namespace std;


vector<map<string , string> > global_database(1000) ; 

const string predefined_username = "manager" ; 
const string  predefined_password = "123456" ; 

  const int BUFFERSIZE = 2048;



void authenticate_manager(int& new_socket, string& authentication_string ){


                char username_buffer[1024] , password_buffer[1024] ; 

                string username_string , password_string  ; 

                read(new_socket , username_buffer , 1024 ) ; 

                username_string = username_buffer ; 

                read(new_socket , password_buffer , 1024 ) ;
                

                password_string = password_buffer ; 

            
                authentication_string = (username_string==predefined_username && password_string == predefined_password)?"authenticated":"unatuhenticated" ; 

                if( send(new_socket ,authentication_string.data() , authentication_string.size() , 0 ) != authentication_string.size() ){
                    perror("authentication_string sending failed");
                }
                

            }


void prompt_acceptance(int& sock , string& authentication_string){
    char prompt_buffer[1024] ;
    recv(sock , prompt_buffer , 1024 , 0 ) ; 
 
    string prompt_string(prompt_buffer) ; 


    if(prompt_string[0] == 'y')
        authenticate_manager(sock,authentication_string) ;  
}



string get_value_guest(string key , int id){

    if(global_database[id][key] != "")
        return global_database[id][key] ; 
    return ("no entry found :("); 
}

string put_value_guest(string key , string value , int id){
  
    global_database[id][key] = value ; 
    return ("added to database (" + key + "," + value + ")"  );
}

string put_value_manager(string key , string value , int id,string authentication_string){
    if(authentication_string != "authenticated" )
        return "you are not logged in as a manager UwU" ;  
    global_database[id][key] = value ;  
    return ("added to database (" + key + "," + value + ") of client " + to_string(id) );
}

string get_value_manager(string key , int id,string authentication_string){

     if(authentication_string != "authenticated" )
        return "you are not logged in as a manager UwU" ;  

    if(global_database[id][key] != "")
        return global_database[id][key] ; 
    return ("no entry found :("); 
}

int main_program(int&& conn){


                vector<string> query_info(10 , "") ; 

            string key , value , extra_string , client_id  , response_string  ; 

             char message[BUFFERSIZE] = {0} , response[BUFFERSIZE] = {0} , query_string[BUFFERSIZE] = {0} ;
             string authentication_string = "" ; 

             prompt_acceptance(conn,authentication_string);
           
            while (recv(conn, query_string, BUFFERSIZE, 0) > 0) {


                int counter = 0 ; 
        

                   // cout<<query_string<<endl ;

                    for(int i = 0 ; i < strlen(query_string)  ; i++ ){

                     //   cout<<counter<<endl ; 
                        
                        if(query_string[i] =='#')
                        {
                            counter++ ; 
                        }
                        else 
                        {
                            query_info[counter] += query_string[i] ;
                        }
                    }

                        printf("received query : ");
                        cout<<query_info[0]<<" "<<query_info[1]<<" "<<query_info[2]<<" "<<query_info[3]<<" "<<query_info[4]<<endl ; 

                        if(query_info[0] == "put"){
                        key = query_info[1] ; 
                        value = query_info[2] ;
                        int id = atoi( query_info[3].c_str() );  
                        //cout<<key<<"          "<<value<<"\n" ; 
                        response_string = put_value_guest(key , value , id ) ; 
                    }
                    else if(query_info[0] == "get" ){
                            key = query_info[1] ; 
                        //value = query_info[2] ;
                        int id = atoi( query_info[3].c_str() );                           
                        response_string = get_value_guest(key , id ) ; 
                    }
                    else if(query_info[0] == "putm"){
                        key = query_info[1] ; 
                        value = query_info[2] ;
                        int id = atoi( query_info[3].c_str() ); 
                        int cli_id =  atoi( query_info[4].c_str() ); 
            
                        response_string = put_value_manager(key , value , cli_id , authentication_string ) ; 
                    }
                    else if(query_info[0] == "getm" ){
                            key = query_info[1] ; 
                        //value = query_info[2] ;
                        int id = atoi( query_info[3].c_str() );  

                         int cli_id =  atoi( query_info[4].c_str() ); 
                         
                            response_string = get_value_manager(key ,  cli_id , authentication_string ) ; 
                      
                    }
                    else if(query_info[0] != "exit" ){
                        response_string = "invalid query :( , try again ." ; 
                    }


                    send(conn, response_string.data() , response_string.size() , 0 ) ; 


                    query_info.clear() ; 
                    query_info.assign(10 , "") ; 
                    response_string ="" ; 
                    bzero(query_string,BUFFERSIZE) ;
                    memset(query_string, 0, sizeof(query_string));
            }
            return 0;

}

int main(int argc,char *argv[]) {

    sockaddr_in serv, client; //main server variable.
    int fd; //Socket file descriptor that will be used to identify the socket
    int conn; //Connection file descriptor that will be used to distinguish client connections.

    char* host_addr = "127.0.0.1" ; 

    int PORT = 8888 ;  

    if(argc>1)
        PORT = atoi(argv[1]) ; 

    if(argc>2)
        host_addr = argv[2] ; 


    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT); //Define the port at which the server will listen for connections.
    serv.sin_addr.s_addr = inet_addr(host_addr); // or use INADDR_ANY

    if((fd = socket(AF_INET, SOCK_STREAM,0) )<0){ //AF_UNIX
        cerr<<"No sock Descriptor"<<endl;
        return -1;
    }

    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        cerr<<"setsockopt";
        return -1;
    }

    if (bind(fd, (struct sockaddr *) &serv, sizeof(serv))< 0){
        cerr<<"Bind error";
        return -1;
    } //assigns the address specified by serv to the socket

    if(listen(fd, 1)<0){
        cout<<"Unable to Listen";
        return -1;
    } //Listen for client connections

    cout<<"Server Running on IP: "<<inet_ntoa(serv.sin_addr)<<" PORT: "<<ntohs(serv.sin_port)<<endl;

//Now we start handling the connections.
    int addlen = sizeof(client);

  
    while (conn = accept(fd, (struct sockaddr *) &client, (socklen_t *)&addlen)){
        int pid;
        cout<<"New Connection with Client IP: "<<inet_ntoa(client.sin_addr)<<" Client PORT: "<<ntohs(client.sin_port)<<endl;
        thread t(main_program,conn) ; 
        t.detach();
    }

    return 0;
}
