#pragma once
#include <cstdint>

class Ballot {
  public:
    Ballot(uint64_t node_id) : node_id_(node_id) { counter_ = 0; };
    inline void Increment() { counter_++; }
    inline uint64_t Counter() { return counter_; }
    inline uint64_t NodeId() { return node_id_; }

  private:
    uint64_t counter_;
    uint64_t node_id_;
};
