#include "split.hpp"
#include "library/algorithm/string/kmp.hpp"

namespace Dawn::Utility {

std::vector<std::string> Split(const std::string& str,
                               const std::string& delim) {
    auto occurences =
        Library::Algorithm::KMPAlgorithm::OccurrenceIndices(str, delim);
    std::vector<std::string> result_{""};
    unsigned int occurence_index = 0;
    for (unsigned int i = 0; i < str.size(); ++i) {
        if (occurence_index < occurences.size() &&
            i == occurences.at(occurence_index)) {
            result_.emplace_back("");
            ++occurence_index;
            i += delim.size() - 1;
            continue;
        }
        result_.back().push_back(str.at(i));
    }
    return result_;
}

}  // namespace Dawn::Utility
