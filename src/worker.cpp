//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "worker.h"
#include "src/network/network.h"
#include "src/utils/utils.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

worker::worker() {
    m_count = 0;
    m_test = 0;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(seed);
    m_color = (color_t) (rand() % 2);
}

void worker::accept_round(int round_number){
    print_log("round number:" + std::to_string(round_number) + ", color: " + std::to_string((int) m_color));
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void worker::query_answer() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    int opt = 1;
    int master_socket , new_socket , client_socket[300] ,
          max_clients = 300 , activity, i , valread , sd;
    int max_sd;
    fd_set readfds;
 
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    // Creating socket file descriptor
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0))
        == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Forcefully attaching socket to the port 8080
    if (setsockopt(master_socket, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    auto ip =  std::string(getenv("IP"));
    auto last_addr = (int) (ip.back() - '0');
    address.sin_port = htons(PORT + last_addr);
 
    //printf("socket bind at %s at %d\n", ip.c_str(), PORT+last_addr);
    // Forcefully attaching socket to the port 8080
    if (bind(master_socket, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(master_socket, 300) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1) {
	//clear the socket set 
        FD_ZERO(&readfds);  
     
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
             
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                 
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))  
        {  
            if ((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
             
            //inform user of socket number - used in send and receive commands 
            //printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
           
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    //printf("Adding to list of sockets as %d\n" , i);  
                         
                    break;  
                }  
            }  
        }  
             
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];  
                 
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                }  
                     
                //Echo back the message that came in 
                else 
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    //printf("%s send color %d\n", ip.c_str(), (int)m_color);
                    //printf("test is %d\n", (int)m_test);

                    std::string msg = std::to_string(m_color);
                    send(sd, msg.c_str(), strlen(msg.c_str()), 0);
                }  
            }  
        }  
    }
    return;
}



void worker::run_snowflake_loop(int round_number){
    int k_sample_size = std::stoi(std::string(getenv("K_SAMPLE_SIZE"))); // TODO ask oren if we want to define it in utils?
    double alpha = std::stod(std::string(getenv("ALPHA"))); // TODO ask oren if we want to define it in utils?
    int beta = std::stoi(std::string(getenv("BETA"))); // TODO ask oren if we want to define it in utils?
    m_test = 1;
    //printf("change test to %d\n", (int)m_test);
    while (true){
        auto k_sample_list = Sample("1", k_sample_size); // TODO change to "m_id"
        auto sample_results = QueryAll(k_sample_list, round_number);
        //std::cout<< "finish query" << std::endl;
        for (auto color : colors) {
            //std::cout<< "count" << std::endl;
            int count = CountSampleResults(sample_results, color);
            //std::cout<< "finish count " << count << std::endl;
            if (count >=  alpha * k_sample_size){
                //std::cout<< "check color" << std::endl;
                if (color != m_color){
                    m_color = color;
                    //std::cout<< "change color" << std::endl;
                    m_count = 0;
                }
                else {
                    m_count++;
                    //std::cout<< "count color " << m_count << std::endl;
                    if(m_count > beta){
                        //std::cout<< "finished round" << std::endl;
                        accept_round(round_number);
                        return;
                    }
                }
            }
        }
    }
}


void worker::run_snowflake(){
    //std::thread query_thread [this](){worker::query_answer();};
    //std::thread query_thread (worker::query_answer);
    //print_log("test");
    //int k_sample_size = std::stoi(std::string(getenv("K_SAMPLE_SIZE"))); // TODO ask oren if we want to define it in utils?
    //auto k_sample_list = Sample("1", 1); // TODO change to "m_id"
    //QueryAll(k_sample_list, 1);
    //exit(0);
    int number_of_rounds = std::stoi(std::string(getenv("NUMBER_OF_ROUNDS")));
    //std::cout<< "number of rounds" << number_of_rounds << std::endl;
    for (int i = 0; i < number_of_rounds ; ++i) {
        run_snowflake_loop(i);
    }
}

