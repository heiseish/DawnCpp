#include "kmp.hpp"

#include <algorithm>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

namespace Dawn::Library::Algorithm {

/**
 * String matching algorithm which runs in O(m + n)
 * Use prefix function
 * The task is the classical application of the prefix function. Given a text t
 * and a string s, we want to find and display the positions of all occurrences
 * of the string s in the text t For convenience we denote with n the length of
 * the string s and with m the length of the text t. We generate the string
 * s+#+t, where # is a separator that appears neither in s nor in t. Let us
 * calculate the prefix function for this string. Now think about the meaning of
 * the values of the prefix function, except for the first n+1 entries (which
 * belong to the string s and the separator). By definition the value π[i] shows
 * the longest length of a substring ending in position i that coincides with
 * the prefix. But in our case this is nothing more than the largest block that
 * coincides with s and ends at position i. This length cannot be bigger than n
 * due to the separator. But if equality π[i]=n is achieved, then it means that
 * the string s appears completely in at this position, i.e. it ends at position
 * i. Just do not forget that the positions are indexed in the string s+#+t Thus
 * if at some position i we have π[i]=n, then at the position i−(n+1)−n+1=i−2n
 * in the string t the string s appears. As already mentioned in the description
 * of the prefix function computation, if we know that the prefix values never
 * exceed a certain value, then we do not need to store the entire string and
 * the entire function, but only its beginning. In our case this means that we
 * only need to store the string s+# and the values of the prefix function for
 * it. We can read one character at a time of the string t and calculate the
 * current value of the prefix function. Thus the Knuth-Morris-Pratt algorithm
 * solves the problem in O(n+m) time and O(n) memory.
 * @arg text text t
 * @arg pattern string s
 * @return list if indices where string s starts in text t
 */
std::vector<unsigned int> KMPAlgorithm::KMP(const std::string_view& text,
                                            const std::string_view& pattern) {
    int n = static_cast<int>(text.length()),
        m = static_cast<int>(pattern.length());
    std::vector<int> pi(std::max(n, m) + 1);
    std::vector<unsigned int> ans;
    int i = 0, j = -1;
    pi[0] = -1;
    while (i < m) {
        while (j >= 0 && pattern[i] != pattern[j]) j = pi[j];
        ++i;
        ++j;
        pi[i] = j;
    }
    i = 0, j = 0;
    while (i < n) {
        while (j >= 0 && text[i] != pattern[j]) j = pi[j];
        ++i;
        ++j;
        if (j == m) {
            ans.emplace_back(i - j);
            j = pi[j];
        }
    }
    return ans;
}

std::vector<unsigned int> KMPAlgorithm::OccurrenceIndices(const char* text,
                                                          const char* pattern) {
    std::string_view first_sv(text, strlen(text));
    std::string_view second_sv(pattern, strlen(pattern));
    return KMP(first_sv, second_sv);
}
std::vector<unsigned int> KMPAlgorithm::OccurrenceIndices(
    const std::string& text, const std::string& pattern) {
    std::string_view first_sv(text.c_str(), text.length());
    std::string_view second_sv(pattern.c_str(), pattern.length());
    return KMP(first_sv, second_sv);
}

bool KMPAlgorithm::Contains(const char* text, const char* pattern) {
    std::string_view first_sv(text, strlen(text));
    std::string_view second_sv(pattern, strlen(pattern));
    auto res = KMP(first_sv, second_sv);
    return !res.empty();
}
bool KMPAlgorithm::Contains(const std::string& text,
                            const std::string& pattern) {
    std::string_view first_sv(text.c_str(), text.length());
    std::string_view second_sv(pattern.c_str(), pattern.length());
    auto res = KMP(first_sv, second_sv);
    return !res.empty();
}

}  // namespace Dawn::Library::Algorithm