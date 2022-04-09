//
// Created by Naor_Shlomo on 09/04/2022.
//
#define BOOST_LOG_DYN_LINK 1
#include "utils.h"
#include <iostream>
//#include <boost/log/trivial.hpp>

void print_log(std::string message){
    //BOOST_LOG_TRIVIAL(info) << message;
    std::cout << message << std::endl;
}

int CountSampleResults(std::map<std::string, color_t > sample_results, color_t color){
    int count = 0;
    for (auto sample: sample_results){
        if (sample.second == color){
            count++;
        }
    }
    return count;
}
