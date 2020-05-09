#include "env.hpp"
#include <cstdlib>
#include <string>

namespace Dawn::Utility {

std::string SafeGetEnv(const char* s)
{
#ifdef WINDOWS
    char buf[1024];
    auto get_res = GetEnvironmentVariableA(name, buf, sizeof(buf));
    if (get_res >= sizeof(buf)) {
        return "TOO_BIG";
    }

    if (get_res == 0) {
        return "UNSET";
    }

    return std::string(buf, get_res);
#else
    if (const char* res = std::getenv(s)) {
        return std::string(res);
    }
    return std::string("");
#endif
}

}  // namespace Dawn::Utility
