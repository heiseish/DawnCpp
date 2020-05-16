#pragma once

#ifndef DAWN_MAIN_FLAGS_H
#define DAWN_MAIN_FLAGS_H

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

void InitArgs(int argc, char** argv);

ABSL_DECLARE_FLAG(std::string, config_folder);
ABSL_DECLARE_FLAG(std::string, config_file);
ABSL_DECLARE_FLAG(std::string, data_folder);

#endif
