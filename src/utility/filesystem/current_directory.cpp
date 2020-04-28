#include "current_directory.hpp"
#include <stdio.h> /* defines FILENAME_MAX */
#include "utility/general/logging.hpp"
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace Dawn::Utility {

std::string GetCurrentWorkingDirectory()
{
    char cCurrentPath[FILENAME_MAX];
    DAWN_ENFORCE(GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)),
                 "Cannot get current working dir");
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
    return std::string(cCurrentPath);
}

}  // namespace Dawn::Utility