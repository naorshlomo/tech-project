//
// Created by Naor_Shlomo on 09/04/2022.
//
#define BOOST_LOG_DYN_LINK 1
#include "utils.h"
#include <iostream>

#include <fstream>

//#include <boost/log/trivial.hpp>
// pod-name_alpha_beta_k_numberofrounds
//std::ofstream g_file ( std::string(getenv("MY_POD_NAME")) + "_" + std::string(getenv("ALPHA")) + "_" + std::string(getenv("BETA")) + "_" + std::string(getenv("K_SAMPLE_SIZE")) + "_" + std::string(getenv("NUMBER_OF_ROUNDS")));
std::ofstream g_file ( std::string(getenv("MY_POD_NAME")) + "_res");

void print_log(std::string message){
    //BOOST_LOG_TRIVIAL(info) << message;

    std::cout << message << std::endl;
}

void print_csv(std::string message){
    //BOOST_LOG_TRIVIAL(info) << message;

    g_file << message << std::endl;
}

int CountSampleResults(std::map<std::string, color_t > &sample_results, color_t color){
    int count = 0;
    for (auto sample: sample_results){
        if (sample.second == color){
            count++;
        }
    }
    return count;
}
