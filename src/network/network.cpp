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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 8080

std::vector<std::string> host_list;

std::string lookup_host (const char *host)
{
    struct addrinfo hints, *res, *result;
    int errcode;
    char addrstr[100] = {0};
    void *ptr;
    std::string return_value;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    errcode = getaddrinfo (host, NULL, &hints, &result);
    if (errcode != 0) {
        perror ("getaddrinfo");
        return "";
    }
    res = result;
    printf ("Host: %s\n", host);
    while (res) {
        inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);
        switch (res->ai_family) {
            case AF_INET:
                ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
                break;
            case AF_INET6:
                ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
                break;
        }
        inet_ntop (res->ai_family, ptr, addrstr, 100);
        return_value = std::string(addrstr);
        printf ("IPv%d address: %s (%s)\n", res->ai_family == PF_INET6 ? 6 : 4,
                addrstr, res->ai_canonname);
        res = res->ai_next;
    }

    freeaddrinfo(result);
    return return_value;
}

void log_sample(std::vector<std::string> result) {
    std::stringstream ss;
    ss << "chosen in Sample are:\n";
    for (auto res: result) {
        ss << "\t" << res << "\n";
    }
    print_log(ss.str());
}

std::vector<std::string> Sample(int k_sample_size) {
    std::vector<std::string> result;
    std::vector<std::string>::iterator position = std::find(host_list.begin(), host_list.end(), std::string(getenv("MY_POD_IP")));
    if (position != host_list.end()) {
        host_list.erase(position);
    }
    std::mt19937 engine;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    engine.seed((unsigned long)seed);
    std::shuffle(host_list.begin(), host_list.end(), engine);
    for (int i = 0; i < k_sample_size; i++) {
        result.push_back(host_list[i]);
    }
    //log_sample(result);
    return result;
}

color_t query(std::string addr, int round_number) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return (color_t)-1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, addr.c_str(), &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return (color_t)-1;
    }
 
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
//        printf("\nConnection Failed \n");
        return (color_t)-1;
    }
    send(sock, std::to_string(round_number).c_str(), strlen(std::to_string(round_number).c_str()), 0);
    read(sock, buffer, 1);
    return (color_t)(buffer[0] - '0');
} 

std::map<std::string, color_t> QueryAll(std::vector<std::string> sample_list, int round_number) {
    std::map<std::string, color_t>  result;
    for (auto node: sample_list) {
        auto addr = lookup_host(node.c_str());
	    auto query_result = query(addr, round_number);
	    if (query_result != -1) {
                result[node] = query_result;
	    }
    }
    return result;
}

int getQuerySocket(int max_clients) {
    struct sockaddr_in address;
    int opt = 1;
    int master_socket;
 
    //initialise all client_socket[] to 0 so not checked

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
    address.sin_port = htons(PORT);
 
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
    return master_socket;
}
