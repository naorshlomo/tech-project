//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "worker.h"
#include "src/network/network.h"
#include "src/utils/utils.h"
#include <stdlib.h>
#include <string>
#include <iostream>

worker::worker() {
    m_count = 0;
    m_color = (color_t) (rand() % 2);
}

void worker::accept(int round_number){
    print_log("round number:" + std::to_string(round_number) + ", color: " + std::to_string((int) m_color));
}

void worker::run_snowflake_loop(int round_number){
    int k_sample_size = std::stoi(std::string(getenv("K_SAMPLE_SIZE"))); // TODO ask oren if we want to define it in utils?
    double alpha = std::stod(std::string(getenv("ALPHA"))); // TODO ask oren if we want to define it in utils?
    int beta = std::stoi(std::string(getenv("BETA"))); // TODO ask oren if we want to define it in utils?
    while (true){
        std::cout<< "in the while" << std::endl;
        auto k_sample_list = Sample("1", k_sample_size); // TODO change to "m_id"
        auto sample_results = QueryAll(k_sample_list, round_number);
        for (auto color : colors) {
            int count = CountSampleResults(sample_results, color);
            if (count >=  alpha * k_sample_size){
                if (color != m_color){
                    m_color = color;
                    m_count = 0;
                }
                else {
                    m_count++;
                    if(m_count > beta){
                        accept(round_number);
                        return;
                    }
                }
            }
        }
    }
}

void worker::run_snowflake(){
    int number_of_rounds = std::stoi(std::string(getenv("NUMBER_OF_ROUNDS")));
    std::cout<< "number of rounds" << number_of_rounds << std::endl;
    for (int i = 0; i < number_of_rounds ; ++i) {
        run_snowflake_loop(i);
    }
}

