//
// Created by Naor_Shlomo on 09/04/2022.
//

#ifndef TECH_PROJECT_WORKER_H
#define TECH_PROJECT_WORKER_H

#include "src/utils/utils.h"
#include <atomic>

class worker {
    std::map<int, int> m_count;
    std::map<int, color_t> m_colors;
public:
    worker();
    void accept_round(int round_number);
    void run_snowflake();
    void run_snowflake_loop(int round_number);
    void queryAnswer();
};

#endif //TECH_PROJECT_WORKER_H
