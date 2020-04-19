#pragma once

#ifndef LIBRARY_NEURALNET_H
#define LIBRARY_NEURALNET_H

#include <string_view>
#include <unordered_map>
#include <vector>

namespace Dawn::Library {

class Network {
public:
    virtual unordered_map<std::string_view, std::vector<float>> Forward(
        unordered_map<std::string_view, std::vector<float>>&& net_input,
        const std::vector<std::string_view>& output_names) = 0;
};

}  // namespace Dawn::Library
#endif