#include "trim.hpp"
#include <regex>
#include <string>

namespace Dawn::Utility {

void Trim(std::string& str)
{
    str = std::regex_replace(str, std::regex("^ +| +$|( ) +"), "$1");
}
void TrimLeading(std::string& str)
{
    str = std::regex_replace(str, std::regex("^ +"), "");
}
void TrimTrailing(std::string& str)
{
    str = std::regex_replace(str, std::regex(" +$"), "");
}

void TrimExtraSpaces(std::string& str)
{
    str = std::regex_replace(str, std::regex(" +"), " ");
}

}  // namespace Dawn::Utility
