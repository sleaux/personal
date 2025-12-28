#pragma once

#include <memory>
#include <mutex>
#include <set>

#include "ballot.h"
#include "change_function.h"
#include "store.h"
#include "transport.h"
class Node;

class Node {
  public:
    Node(uint64_t, std::unique_ptr<Store>);
    void AddTransport(std::unique_ptr<Transport>);

    template <typename... Nodes>
    inline void AddNodes(Nodes... nodes) {
        nodes_ = {nodes...};
    }
    PrepareResponse Prepare(PrepareRequest);
    AcceptResponse Accept(AcceptRequest);
    std::byte Propose(std::byte, ChangeFunction);
    inline std::shared_ptr<Transport> Transport() { return transport_; }

  private:
    std::byte SendPrepare(std::byte);
    std::byte SendAccept(std::byte, std::byte, ChangeFunction);

    inline std::byte KeyBallotAccepted(std::byte key) {
        return static_cast<std::byte>(static_cast<unsigned char>(key) + 100);
    }

    inline std::byte KeyBallotPromised(std::byte key) {
        return static_cast<std::byte>(static_cast<unsigned char>(key) + 200);
    }

    uint64_t id_;
    Ballot ballot_;
    std::set<std::shared_ptr<Node>> nodes_;
    std::mutex mutex_;
    std::unique_ptr<Store> store_;
    std::shared_ptr<class ::Transport> transport_;
};

template <typename... Nodes>
inline void MingleNodes(Nodes... nodes) {
    (std::forward<Nodes>(nodes)->AddNodes(nodes), ...);
}
