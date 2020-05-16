#include "config.hpp"

namespace Dawn {

Config* Config::s_instance = nullptr;

Config::Config(const std::string& config_path, ConfigSourceType source)
{
    switch (source) {
        case ConfigSourceType::Path: {
            std::ifstream _config_file(config_path);
            _config_file >> _config;
        } break;

        case ConfigSourceType::RawString:
            _config = json::parse(config_path);
            break;
    }
}

Config* Config::Instance()
{
    if (!s_instance)
        s_instance = new Config(absl::GetFlag(FLAGS_config_file));
    return s_instance;
}

}  // namespace Dawn