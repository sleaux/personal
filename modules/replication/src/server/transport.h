#pragma once
#include <cstddef>

#include "ballot.h"

class PrepareRequest {
  public:
    PrepareRequest(Ballot ballot, std::byte key) : ballot_(ballot), key_(key) {}
    inline Ballot Ballot() { return ballot_; }
    inline std::byte Key() { return key_; }

  private:
    class ::Ballot ballot_;
    std::byte key_;
};

class PrepareResponse {
  public:
    PrepareResponse() {}

  private:
};

class AcceptRequest {
  public:
    AcceptRequest(Ballot ballot, std::byte key, std::byte state_new)
        : ballot_(ballot), key_(key), state_new_(state_new) {}
    inline Ballot Ballot() { return ballot_; }
    inline std::byte Key() { return key_; }
    inline std::byte StateNew() { return state_new_; }

  private:
    class ::Ballot ballot_;
    std::byte key_;
    std::byte state_new_;
};

class AcceptResponse {
  public:
    AcceptResponse() {}

  private:
};

class Transport {
  public:
    virtual PrepareResponse SendPrepare(PrepareRequest) = 0;
    virtual AcceptResponse SendAccept(AcceptRequest) = 0;
    virtual ~Transport() {};
};
