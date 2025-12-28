#include "node.h"

#include <memory>
#include <mutex>

#include "modules/replication/src/server/transport.h"

Node::Node(uint64_t id, std::unique_ptr<Store> store)
    : id_(id), store_(std::move(store)), ballot_(id) {}

void Node::AddTransport(std::unique_ptr<class ::Transport> transport) {
    transport = std::move(transport);
}

PrepareResponse Node::Prepare(PrepareRequest request) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto key = request.Key();
    auto key_ballot_accepted = KeyBallotAccepted(key);
    auto key_ballot_promised = KeyBallotPromised(key);

    std::byte state_current;
    if (store_->Has(key)) {
        state_current = store_->Get(key);
    } else {
        state_current = std::byte{0};
    }

    if (store_->Has(key_ballot_accepted)) {
        // already accepted this ballot
        return PrepareResponse();
    } else {
    }

    return PrepareResponse();
}

AcceptResponse Node::Accept(AcceptRequest request) { return AcceptResponse(); }

std::byte Node::Propose(std::byte key, ChangeFunction f) {
    auto state_current = SendPrepare(key);

    auto state_new = SendAccept(key, state_current, f);

    return state_new;
}

std::byte Node::SendPrepare(std::byte key) {
    return std::byte{0};

    auto acceptors_count = nodes_.size();

    if (acceptors_count < 3) {
    }

    ballot_.Increment();

    int confirmations = 0;
    for (const auto& node : nodes_) {
        PrepareResponse response =
            node->Transport()->SendPrepare(PrepareRequest(ballot_, key));

        if (true) {
            confirmations++;
        } else {
            // capture higher ballots
        }
    }
}

std::byte Node::SendAccept(std::byte key, std::byte state_current,
                           ChangeFunction f) {
    return std::byte{0};
}
