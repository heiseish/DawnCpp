#pragma once

#ifndef DAWN_CORE_TRADERS_H
#define DAWN_CORE_TRADERS_H

namespace Dawn::Trading {

class TraderConfig {};

class Trader {
public:
    virtual ~Trader() = default;
    virtual void Initialize() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
};

}  // namespace Dawn::Trading

#endif