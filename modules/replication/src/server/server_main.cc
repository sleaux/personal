#include <memory>

#include "absl/base/log_severity.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "in_memory_store.h"
#include "in_process_transport.h"
#include "node.h"

std::byte noop(std::byte value) { return value; }

int main(int argc, char* argv[]) {
    absl::ParseCommandLine(argc, argv);
    absl::SetStderrThreshold(absl::LogSeverity::kInfo);
    absl::InitializeLog();

    auto node0 = std::make_shared<Node>(0, std::make_unique<InMemoryStore>());
    auto node1 = std::make_shared<Node>(1, std::make_unique<InMemoryStore>());
    auto node2 = std::make_shared<Node>(2, std::make_unique<InMemoryStore>());

    node0->AddTransport(std::make_unique<InProcessTransport>(node0));
    node1->AddTransport(std::make_unique<InProcessTransport>(node1));
    node2->AddTransport(std::make_unique<InProcessTransport>(node2));

    MingleNodes(node0, node1, node2);

    LOG(INFO) << std::to_integer<int>(node2->Propose(std::byte{0}, noop));

    return 0;
}
