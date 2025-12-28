#pragma once
#include <cstddef>

class Store {
  public:
    virtual void Set(std::byte key, std::byte value) = 0;
    virtual std::byte Get(std::byte key) = 0;
    virtual bool Has(std::byte key) = 0;
    virtual ~Store() {};
};
