//
// Created by Naor_Shlomo on 09/04/2022.
//

#ifndef TECH_PROJECT_NETWORK_H
#define TECH_PROJECT_NETWORK_H

#include "src/utils/utils.h"
#include <map>
#include <vector>
#include <string>

std::vector<std::string> Sample(std::string current, int k_sample_size);
std::map<std::string, color_t> QueryAll(std::vector<std::string> sample_list, int round_number);

#endif //TECH_PROJECT_NETWORK_H
