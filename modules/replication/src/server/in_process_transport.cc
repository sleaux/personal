#include "in_process_transport.h"

#include "transport.h"

InProcessTransport::InProcessTransport(std::shared_ptr<Node> node)
    : node_(node) {}

PrepareResponse InProcessTransport::SendPrepare(PrepareRequest request) {
    return node_->Prepare(request);
}

AcceptResponse InProcessTransport::SendAccept(AcceptRequest request) {
    return node_->Accept(request);
}
