//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "worker.h"
#include "utils/utils.h"
#include "network/network.h"
#include <thread>
#include <chrono>
#include <random>

#define PORT 8080
#define PORT_2 8081

int REPLICAS = std::stoi(std::string(getenv("REPLICAS")));
int K_SAMPLE_SIZE = std::stoi(std::string(getenv("K_SAMPLE_SIZE")));
double ALPHA = std::stod(std::string(getenv("ALPHA")));
int BETA = std::stoi(std::string(getenv("BETA")));

void QueryAnswerThread(worker& a, int local_port) {
    a.queryAnswer(local_port);
}



int main() {
    worker my_worker;
    std::thread query_answer_thread (QueryAnswerThread, std::ref(my_worker), PORT);
//    std::thread query_answer_thread_2 (QueryAnswerThread, std::ref(my_worker), PORT_2);
    std::this_thread::sleep_for(std::chrono::milliseconds(20000));

    for (int i = 0; i < std::stoi(std::string(getenv("REPLICAS"))) ; ++i) {
        std::string new_host = "worker-envars-fieldref-statefulset-" + std::to_string(i) + ".worker.default.svc.cluster.local";

        host_list.push_back(new_host);
        auto addr = lookup_host(new_host.c_str());
        if (addr !=std::string(getenv("MY_POD_IP")) ) {
            ip_list.push_back(addr);
            for(int j = 0; j < BATCH_SIZE; j++) {
//                std::random_device rd;
//                std::mt19937 mt(rd());
//                std::uniform_real_distribution<double> dist(-10.0, 10.0);
//                int local_port = dist(mt) >= 0 ? PORT : PORT_2;
                auto new_addr = addr + std::to_string(j);
//                socket_list[new_addr] = get_socket(addr, local_port);
                socket_list[new_addr] = get_socket(addr, PORT);
                print_log("adding new host " + new_host + " with ip " + addr + "and port " + std::to_string(PORT));
                print_log("added " + addr);
            }
        } else{
            print_log("removed my ip " + addr);
        }

    }

    print_log("started running pod: " + std::string(getenv("MY_POD_NAME")));


    my_worker.run_snowflake();
    query_answer_thread.join();
//    query_answer_thread_2.join();
    return 0;
}
