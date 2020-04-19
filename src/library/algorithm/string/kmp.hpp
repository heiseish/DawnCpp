#pragma once

#ifndef ALGORITHM_KMP_H
#define ALGORITHM_KMP_H

#include <string>
#include <string_view>
#include <vector>

#include "library/algorithm/basic_algorithm.hpp"

namespace Dawn::Library::Algorithm {

// TODO Make case sensitivity toggleable
class KMPAlgorithm final : public BasicAlgorithm {
public:
    static constexpr std::string_view Name = "KMP";
    static std::vector<unsigned int> OccurrenceIndices(const char*,
                                                       const char*);
    static std::vector<unsigned int> OccurrenceIndices(const std::string&,
                                                       const std::string&);
    /**
     * Check if `_text` contains a string as substring
     */
    static bool Contains(const char*, const char*);
    static bool Contains(const std::string&, const std::string&);

private:
    static std::vector<unsigned int> KMP(const std::string_view&,
                                         const std::string_view&);
};

}  // namespace Dawn::Library::Algorithm

#endif