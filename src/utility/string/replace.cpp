#include "replace.hpp"
#include <algorithm>
#include <string>
#include "library/algorithm/string/kmp.hpp"
#include "utility/general/logging.hpp"
namespace Dawn::Utility {

std::string Replace(const std::string& str,
                    const std::string& substr_to_replace,
                    const std::string& replacement,
                    unsigned int offset,
                    [[maybe_unused]] const bool& case_sensitive)
{
    if (offset >= str.size())
        return str;
    auto occurences = Library::Algorithm::KMPAlgorithm::OccurrenceIndices(
        str, substr_to_replace);
    if (occurences.empty())
        return str;
    auto idx_iter =
        std::lower_bound(occurences.begin() + offset, occurences.end(), offset);
    if (idx_iter == occurences.end())
        return str;
    std::string result_;
    for (unsigned int i = 0; i < *idx_iter; ++i) {
        result_.push_back(str.at(i));
    }
    result_.append(replacement);
    for (unsigned int i = (*idx_iter) + substr_to_replace.size();
         i < str.size();
         ++i) {
        result_.push_back(str.at(i));
    }
    return result_;
}

std::string _replace_all_chars(const std::string& str,
                               const std::string& char_to_replace,
                               const std::string& replacement,
                               [[maybe_unused]] const bool& case_sensitive)
{
    size_t index = 0;
    auto res = str;
    while (true) {
        /* Locate the substring to replace. */
        index = res.find(char_to_replace, index);
        if (index == std::string::npos)
            break;
        /* Make the replacement. */
        res.replace(index, 1, replacement);
        /* Advance index forward so the next iteration doesn't pick it up as
         * well. */
        index++;
    }
    return res;
}

// TODO case-insensitive
std::string ReplaceAll(const std::string& str,
                       const std::string& substr_to_replace,
                       const std::string& replacement,
                       [[maybe_unused]] const bool& case_sensitive)
{
    if (substr_to_replace.length() == 1 && replacement.length() == 1) {
        return _replace_all_chars(
            str, substr_to_replace, replacement, case_sensitive);
    }
    auto occurences = Library::Algorithm::KMPAlgorithm::OccurrenceIndices(
        str, substr_to_replace);
    if (occurences.empty())
        return str;
    unsigned int occurence_index = 0;
    std::string result;
    for (unsigned int i = 0; i < str.length(); ++i) {
        if (occurence_index < occurences.size() &&
            i == occurences.at(occurence_index)) {
            result += replacement;
            i += substr_to_replace.size() - 1;
            ++occurence_index;
            continue;
        }
        result += str.at(i);
    }
    return result;
}

}  // namespace Dawn::Utility
