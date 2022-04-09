//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "worker.h"
#include "src/network/network.h"
#include "src/utils/utils.h"
#include <stdlib.h>
#include <string>
#include <iostream>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

worker::worker() {
    m_count = 0;
    m_color = (color_t) (rand() % 2);
}

void worker::accept_round(int round_number){
    print_log("round number:" + std::to_string(round_number) + ", color: " + std::to_string((int) m_color));
}

void worker::query_answer() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
 
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
        == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    read(new_socket, buffer, 1024);
    printf("send color %d\n", (int)m_color);

    std::string msg = std::to_string(m_color);
    send(new_socket, msg.c_str(), strlen(msg.c_str()), 0);
    return;
}



void worker::run_snowflake_loop(int round_number){
    int k_sample_size = std::stoi(std::string(getenv("K_SAMPLE_SIZE"))); // TODO ask oren if we want to define it in utils?
    double alpha = std::stod(std::string(getenv("ALPHA"))); // TODO ask oren if we want to define it in utils?
    int beta = std::stoi(std::string(getenv("BETA"))); // TODO ask oren if we want to define it in utils?
    while (true){
        std::cout<< "in the while" << std::endl;
        auto k_sample_list = Sample("1", k_sample_size); // TODO change to "m_id"
        auto sample_results = QueryAll(k_sample_list, round_number);
        for (auto color : colors) {
            int count = CountSampleResults(sample_results, color);
            if (count >=  alpha * k_sample_size){
                if (color != m_color){
                    m_color = color;
                    m_count = 0;
                }
                else {
                    m_count++;
                    if(m_count > beta){
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
    print_log("test");
    int k_sample_size = std::stoi(std::string(getenv("K_SAMPLE_SIZE"))); // TODO ask oren if we want to define it in utils?
    auto k_sample_list = Sample("1", k_sample_size); // TODO change to "m_id"
    QueryAll(k_sample_list, 1);
    return;
    int number_of_rounds = std::stoi(std::string(getenv("NUMBER_OF_ROUNDS")));
    std::cout<< "number of rounds" << number_of_rounds << std::endl;
    for (int i = 0; i < number_of_rounds ; ++i) {
        run_snowflake_loop(i);
    }
}

