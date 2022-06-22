//
// Created by Naor_Shlomo on 09/04/2022.
//

#ifndef TECH_PROJECT_WORKER_H
#define TECH_PROJECT_WORKER_H

#include "src/utils/utils.h"
#include <atomic>
#include <vector>

class worker {
public:
    std::map<int, int> m_count;
    std::map<int, color_t> m_colors;

    worker();
    void accept_round(int round_number);
    void run_snowflake();
    void queryAnswer();
};

void run_snowflake_loop(worker *our_worker, int round_number, std::vector<std::string> local_ip_list);
extern int K_SAMPLE_SIZE;
extern double ALPHA;
extern int BETA;
#endif //TECH_PROJECT_WORKER_H
