#pragma once

#ifndef DAWN_D_RANDOM_H
#define DAWN_D_RANDOM_H

#include <random>

namespace Dawn::Utility {

std::mt19937 &GetRandomGenerator();

}  // namespace Dawn::Utility

#endif