//
// Created by Naor_Shlomo on 09/04/2022.
//

#ifndef TECH_PROJECT_WORKER_H
#define TECH_PROJECT_WORKER_H

#include "src/utils/utils.h"

class worker {
    int m_count;
    color_t m_color;
public:
    worker();
    void accept(int round_number);
    void run_snowflake();
    void run_snowflake_loop(int round_number);
};

#endif //TECH_PROJECT_WORKER_H
