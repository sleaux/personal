#pragma once
#include "node.h"
#include "transport.h"

class InProcessTransport : public Transport {
  public:
    InProcessTransport(std::shared_ptr<Node>);
    PrepareResponse SendPrepare(PrepareRequest) override;
    AcceptResponse SendAccept(AcceptRequest) override;

  private:
    std::shared_ptr<Node> node_;
};
