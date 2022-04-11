//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "worker.h"
#include <thread>
#include <chrono>

void SimpleThread(worker *a) {
    a->query_answer();
}

int main() {
    //worker my_worker = worker();
    worker *my_worker = new worker();
    std::thread query_thread (SimpleThread, my_worker);
    std::this_thread::sleep_for(std::chrono::milliseconds(8000));
    my_worker->run_snowflake();
    query_thread.join();
    return 0;
}
