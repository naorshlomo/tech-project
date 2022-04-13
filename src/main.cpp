//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "worker.h"
#include "utils/utils.h"
#include <thread>
#include <chrono>

void QueryAnswerThread(worker& a) {
    a.queryAnswer();
}

int main() {
    worker my_worker;
    print_log("started running pod: " + std::string(getenv("MY_POD_NAME")));
    std::thread query_answer_thread (QueryAnswerThread, std::ref(my_worker));
    std::this_thread::sleep_for(std::chrono::milliseconds(8000));
    my_worker.run_snowflake();
    query_answer_thread.join();
    return 0;
}
