#include "from_json_file.hpp"
#include <fstream>
#include <sstream>

namespace Dawn::Utility {

std::string FromJsonFile(const std::string& file_path) {
    std::ifstream file(file_path);
    std::ostringstream tmp;
    tmp << file.rdbuf();
    return tmp.str();
}

}  // namespace Dawn::Utility
