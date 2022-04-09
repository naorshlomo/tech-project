//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "worker.h"
#include <thread>
#include <chrono>

void SimpleThread(worker a) {
    a.query_answer();
}

int main() {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    worker my_worker = worker();
    std::thread query_thread (SimpleThread, std::ref(my_worker));
    my_worker.run_snowflake();
    query_thread.join();
    return 0;
}
