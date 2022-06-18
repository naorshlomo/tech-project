//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "worker.h"
#include "utils/utils.h"
#include "network/network.h"
#include <thread>
#include <chrono>



void QueryAnswerThread(worker& a) {
    a.queryAnswer();
}

int main() {
    for (int i = 0; i < std::stoi(std::string(getenv("REPLICAS"))) ; ++i) {
        std::string new_host = "worker-envars-fieldref-statefulset-" + std::to_string(i) + ".worker.default.svc.cluster.local";
        print_log("adding new host " + new_host);
        host_list.push_back(new_host);
        auto addr = lookup_host(new_host.c_str());
        if (addr !=std::string(getenv("MY_POD_IP")) ) {
            ip_list.push_back(addr);
            print_log("added " + addr);
        } else{
            print_log("removed my ip " + addr);
        }
    }

    worker my_worker;
    print_log("started running pod: " + std::string(getenv("MY_POD_NAME")));
    std::thread query_answer_thread (QueryAnswerThread, std::ref(my_worker));
    std::this_thread::sleep_for(std::chrono::milliseconds(20000));
    my_worker.run_snowflake();
    query_answer_thread.join();
    return 0;
}
