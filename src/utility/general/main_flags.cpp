#include "main_flags.hpp"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

void InitArgs([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    absl::ParseCommandLine(argc, argv);
}

ABSL_FLAG(std::string,
          config_folder,
          "",
          "Path to folder containing the config json");

ABSL_FLAG(std::string,
          data_folder,
          "",
          "Path to folder containing the excahnge json");