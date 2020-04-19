#pragma once

#ifndef DAWN_CORE_TRADING_STRATEGY_H
#define DAWN_CORE_TRADING_STRATEGY_H

namespace Dawn::Trading {

class Strategy {
public:
    virtual void IngestData() = 0;
};

}  // namespace Dawn::Trading

#endif