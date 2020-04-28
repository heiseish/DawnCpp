#pragma once

#ifndef DAWN_TRADING_STRAT_INDICATOR_H
#define DAWN_TRADING_STRAT_INDICATOR_H

#include <string_view>
#include <vector>
namespace Dawn::Trading {

class Indicator : public LinesHolder, public MetaIndicator {
public:
    std::vector<std::string_view> Inputs;

    Indicator()
        : autosuper(true),
          _minperiod(1),
          _minperiods(1, _minperiod),
          Inputs({"close"})
    {
    }
    virtual TALib() = 0;

protected:
    bool _autosuper;
    int32_t _minperiod;
    std::vector<int32_t> _minperiods;
};

}  // namespace Dawn::Trading

#endif