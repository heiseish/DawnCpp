#pragma once

#ifndef DAWN_CORE_TRADER_MANAGER_H
#define DAWN_CORE_TRADER_MANAGER_H

#include <memory>
#include <vector>
#include "trader.hpp"

namespace Dawn::Trading {

class TraderManager final {
public:
    void InitializeTraders();
    void ActivateAllTraders();
    void StopAllTraders();

private:
    std::vector<std::unique_ptr<Trader>> _traders;
};

}  // namespace Dawn::Trading

#endif