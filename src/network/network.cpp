//
// Created by Naor_Shlomo on 09/04/2022.
//

#include "network.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>

std::vector<std::string> ip_list = {"192.168.55.2", "192.168.55.3", "192.168.55.4"};

void log_sample(std::vector<std::string> result) {
    std::stringstream ss;
    ss << "chosen in Sample are:\n";
    for (auto res: result) {
        ss << "\t" << res << "\n";
    }
    print_log(ss.str());

}

std::vector<std::string> Sample(std::string current, int k_sample_size){
    std::vector<std::string> result;

    std::vector<std::string>::iterator position = std::find(ip_list.begin(), ip_list.end(), std::string(getenv("IP")));
    if (position != ip_list.end()) {
       ip_list.erase(position);
    }
    std::mt19937 engine;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    engine.seed((unsigned long)seed);
    std::shuffle(ip_list.begin(), ip_list.end(), engine);
    for (int i = 0; i < k_sample_size; i++) {
        result.push_back(ip_list[i]);
    }
    log_sample(result);
    return result;
}

std::map<std::string, color_t> QueryAll(std::vector<std::string> sample_list, int round_number){
    std::map<std::string, color_t>  b;
    return b;
}
