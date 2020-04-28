#include "utility/string/from_json_file.hpp"
#include <rapidjson/document.h>
#include <filesystem>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "utility/filesystem/current_directory.hpp"
namespace Dawn::Utility {

namespace {
namespace fs = std::filesystem;
}
class FromJsonFileTest : public ::testing::Test {
};

TEST_F(FromJsonFileTest, GetContentCorrectly)
{
    auto config_path = fs::path(GetCurrentWorkingDirectory());
    config_path /= "../config/test.json";
    auto res = FromJsonFile(config_path);
    EXPECT_FALSE(res.empty());
    rapidjson::Document doc;
    if (doc.Parse(res.c_str()).HasParseError()) {
        FAIL() << "Invalid json read in\n";
    }
    EXPECT_TRUE(doc.IsObject());
    EXPECT_TRUE(doc.HasMember("name"));
    EXPECT_TRUE(doc["name"].IsString());
    std::string name = doc["name"].GetString();
    EXPECT_EQ(name, "test");
}

}  // namespace Dawn::Utility