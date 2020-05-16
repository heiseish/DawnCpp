#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include "absl/flags/flag.h"
#include "utility/general/main_flags.hpp"

namespace {

template <typename... Ts>
using AllStrings = typename std::enable_if<
    std::conjunction<std::is_convertible<Ts, std::string>...>::value>::type;
using json = nlohmann::json;

}  // namespace

namespace Dawn {

enum ConfigSourceType { Path, RawString };

class Config {
private:
    friend class ConfigTest;
    json _config;
    static Config* s_instance;

    Config(const std::string& config_path,
           ConfigSourceType source = ConfigSourceType::Path);

public:
    template <typename ResultType,
              typename... Args,
              typename = AllStrings<Args...>>
    ResultType Get(Args&&... args)
    {
        json& res = _config;
        ((res = res[args]), ...);
        return res.get<ResultType>();
    }
    static Config* Instance();
};

}  // namespace Dawn