#include "logging.hpp"

namespace Dawn {
void InitLogger()
{
    spdlog::set_pattern(
        "\033[0;95m%c\033[0m - %^[ %l ]%$ - %! - \033[0;96m%s:%#\033[0m: %v");
}

}  // namespace Dawn