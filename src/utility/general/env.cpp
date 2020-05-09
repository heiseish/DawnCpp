#include "env.hpp"
#include <cstdlib>
#include <string>
#include "logging.hpp"
namespace Dawn::Utility {

std::string SafeGetEnv(const char* s)
{
#ifdef WINDOWS
    char buf[1024];
    auto get_res = GetEnvironmentVariableA(name, buf, sizeof(buf));
    DAWN_ENFORCE(get_res <= sizeof(buf), "Directory path name is too bug");
    DAWN_ENFORCE(get_res != 0, "Directory path name is too bug");
    return std::string(buf, get_res);
#else
    if (const char* res = std::getenv(s)) {
        return std::string(res);
    }
    return std::string("");
#endif
}

}  // namespace Dawn::Utility
