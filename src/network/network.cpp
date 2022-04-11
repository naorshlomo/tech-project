//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "network.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

std::vector<std::string> ip_list = {"192.168.55.2", "192.168.55.3", "192.168.55.4"};

void log_sample(std::vector<std::string> result) {
    std::stringstream ss;
    ss << "chosen in Sample are:\n";
    for (auto res: result) {
        ss << "\t" << res << "\n";
    }
    print_log(ss.str());

}

std::vector<std::string> Sample(std::string current, int k_sample_size){
    std::vector<std::string> result;

    std::vector<std::string>::iterator position = std::find(ip_list.begin(), ip_list.end(), std::string(getenv("IP")));
    //if (position != ip_list.end()) {
    ip_list.erase(position);
    //}
    std::mt19937 engine;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    engine.seed((unsigned long)seed);
    std::shuffle(ip_list.begin(), ip_list.end(), engine);
    for (int i = 0; i < k_sample_size; i++) {
        result.push_back(ip_list[i]);
    }
    log_sample(result);
    return result;
}

color_t query(std::string addr, int round_number) {
    //printf("query %s \n", addr.c_str());
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string hello = "Hello from client";
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return (color_t)0;
    }
 
    serv_addr.sin_family = AF_INET;

    auto last_addr = (int) (addr.back() - '0');
    serv_addr.sin_port = htons(PORT + last_addr);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, addr.c_str(), &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return (color_t)0;
    }
 
    //printf("connect %s at %d\n", addr.c_str(), PORT+last_addr);
    if (connect(sock, (struct sockaddr*)&serv_addr,
                sizeof(serv_addr))
        < 0) {
        printf("\nConnection Failed \n");
        return (color_t)0;
    }
    //printf("connected %s at %d\n", addr.c_str(), PORT+last_addr);
    send(sock, hello.c_str(), strlen(hello.c_str()), 0);
    read(sock, buffer, 1024);
    printf("recieved color %d from %s\n", (int) (buffer[0] - '0'), addr.c_str());
    return (color_t)buffer[0];
} 

std::map<std::string, color_t> QueryAll(std::vector<std::string> sample_list, int round_number){
    std::map<std::string, color_t>  result;
    for (auto node: sample_list) {
        result[node] = query(node, round_number);
    }
    return result;
}
