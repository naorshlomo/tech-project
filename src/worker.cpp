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

worker::worker(){
    int number_of_rounds = std::stoi(std::string(getenv("NUMBER_OF_ROUNDS")));
    for (int i = 0; i < number_of_rounds; i++) {
        m_count[i] = 0;
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::srand(seed);
        m_colors[i] = (color_t) (rand() % 2);
    }
}

void worker::accept_round(int round_number){
    print_log("Accepted color: " + std::to_string((int) m_colors.at(round_number)) + " in round number:" + std::to_string(round_number));
    print_csv( std::to_string(round_number) + "," + std::to_string((int) m_colors.at(round_number)));
}

void worker::queryAnswer() {
    char buffer[10] = {0};
    int max_clients = 300;
    int client_socket[300] = {0};
    fd_set readfds;
    int master_socket = getQuerySocket(max_clients);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);

        int max_sd = master_socket;
        for (int i = 0; i < max_clients; i++) {
            int sd = client_socket[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            printf("select error");
        }
        if (FD_ISSET(master_socket, &readfds)) {
            int new_socket = accept(master_socket, NULL, NULL);
            if (new_socket < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }
        for (int i = 0; i < max_clients; i++) {
            int sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                int valread = read(sd, buffer, 10);
                if (valread == 0) {
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    int requested_round = atoi(buffer);
                    std::string msg = std::to_string(m_colors.at(requested_round));
                    send(sd, msg.c_str(), strlen(msg.c_str()), 0);
                    close(sd);
                    client_socket[i] = 0;
                }
            }
        }
    }
    return;
}



void worker::run_snowflake_loop(int round_number){
    int k_sample_size = std::stoi(std::string(getenv("K_SAMPLE_SIZE")));
    double alpha = std::stod(std::string(getenv("ALPHA")));
    int beta = std::stoi(std::string(getenv("BETA")));
    while (true){
        auto k_sample_list = Sample(k_sample_size);
        auto sample_results = QueryAll(k_sample_list, round_number);
        for (auto color : colors) {
            int count = CountSampleResults(sample_results, color);
            if (count >=  alpha * k_sample_size){
                if (color != m_colors.at(round_number)){
                    m_colors[round_number] = color;
                    m_count[round_number] = 0;
                }
                else {
                    m_count[round_number]++;
                    if(m_count[round_number] > beta){
                        accept_round(round_number);
                        return;
                    }
                }
            }
        }
    }
}


void worker::run_snowflake(){
    int number_of_rounds = std::stoi(std::string(getenv("NUMBER_OF_ROUNDS")));
    for (int i = 0; i < number_of_rounds ; ++i) {
        run_snowflake_loop(i);
    }
}

