#pragma once

#ifndef DAWN_TRADING_STRAT_INDICATORS_ATR_
#define DAWN_TRADING_STRAT_INDICATORS_ATR_

#include <string_view>
#include "indicator.hpp"
#include "smma.hpp"


/**
 * @brief Defined by J. Welles Wilder, Jr. in 1978 in his book *"New Concepts in
 * Technical Trading Systems"* for the ATR
 * Records the "true high" which is the maximum of today's high and yesterday's close
 * @formular
 *          - truehigh = max(high, close_prev)
 * @see http://en.wikipedia.org/wiki/Average_true_range
*/
class TrueHigh : public Indicator {
public:
    constexpr static std::string_view Group = "volatility";
    const static std::string_view Alias[4] =  {"TR", "TrueRange", "trange", "TRANGE"}
    const static std::string_view Inputs[4] =  {"low", "close"}
    const static std::string_view Outputs[1] = {"truehi"}
    
}
class truehigh(Indicator):
    '''
    
    '''
    group = 'volatility'
    alias = 'TR', 'TrueRange', 'trange', 'TRANGE'
    inputs = 'low', 'close'
    outputs = 'truehi'
    params = (
        ('_period', 1, 'Period to consider'),
    )

    def __init__(self):
        self.o.truehi = self.i.close(-self.p._period).clip(lower=self.i.high)


class truelow(Indicator):
    '''
    Defined by J. Welles Wilder, Jr. in 1978 in his book *"New Concepts in
    Technical Trading Systems"* for the ATR

    Records the "true low" which is the minimum of today's low and
    yesterday's close

    Formula:
      - truelow = min(low, close_prev)

    See:
      - http://en.wikipedia.org/wiki/Average_true_range
    '''
    group = 'volatility'
    alias = 'TR', 'TrueRange', 'trange', 'TRANGE'
    inputs = 'low', 'close'
    outputs = 'truelo'
    params = (
        ('_period', 1, 'Period to consider'),
    )

    def __init__(self):
        self.o.truelo = self.i.close(-self.p._period).clip(upper=self.i.low)


class truerange(Indicator):
    '''
    Defined by J. Welles Wilder, Jr. in 1978 in his book New Concepts in
    Technical Trading Systems.

    Formula:
      - max(high - low, abs(high - prev_close), abs(prev_close - low)

      which can be simplified to

      - truerange = max(high, prev_close) - min(low, prev_close)

    See:
      - http://en.wikipedia.org/wiki/Average_true_range

    The idea is to take the previous close into account to calculate the range
    if it yields a larger range than the daily range (High - Low)
    '''
    group = 'volatility'
    alias = 'TR', 'TrueRange', 'trange', 'TRANGE'
    inputs = 'high', 'low', 'close'
    outputs = 'tr'
    params = (
        ('_period', 1, 'Period for high/low vs close for truerange calc'),
    )

    def __init__(self):
        close1 = self.i.close(-self.p._period)
        truehi = close1.clip(lower=self.i.high)  # max of close(-1) and hi
        truelo = close1.clip(upper=self.i.low)  # min of close(-1) and low
        self.o.tr = truehi - truelo


class atr(truerange, outputs_override=True):
    '''
    Defined by J. Welles Wilder, Jr. in 1978 in his book *"New Concepts in
    Technical Trading Systems"*.

    The idea is to take the close into account to calculate the range if it
    yields a larger range than the daily range (High - Low)

    Formula:
      - truerange = max(high, close(-1)) - min(low, close(-1))
      - atr = SmoothedMovingAverage(truerange, period)

    See:
      - http://en.wikipedia.org/wiki/Average_true_range
    '''
    group = 'volatility'
    alias = 'ATR', 'AverageTrueRange'
#outputs = {'atr' : 'tr' } #define atr / alias tr to it for the base class
    outputs = 'atr'  # outputs_override in class def, autoalias tr => atr added
    params = (
        ('period', 14, 'Period to consider'),
        ('_ma', smma, 'Moving average to use'),
    )

    def __init__(self):
        self.o.atr = self.p._ma(self.o.tr, period=self.p.period)

#endif