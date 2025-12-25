#include <condition_variable>
#include <mutex>
#include <optional>
#include <string>

#include "absl/base/log_severity.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "modules/replication/proto/caspaxos.grpc.pb.h"

ABSL_FLAG(std::string, target, "localhost:50051", "Server address");
ABSL_FLAG(std::optional<int32_t>, value, std::nullopt, "Value to set");

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class Client {
  public:
    Client(std::shared_ptr<Channel> channel)
        : stub(caspaxos::Server::NewStub(channel)) {}

    void Change(std::optional<int32_t> value) {
        ClientContext context;
        caspaxos::ChangeRequest request;
        if (value.has_value()) {
            request.set_value(value.value());
        }
        caspaxos::ChangeResponse response;
        std::mutex mu;
        std::condition_variable cv;
        bool done = false;
        stub->async()->Change(
            &context, &request, &response,
            [&mu, &cv, &done, &response, this](Status status) {
                bool ret;
                if (!status.ok()) {
                    LOG(INFO) << "Change rpc failed.";
                } else {
                    LOG(INFO) << "Change rpc ok, returned " << response.value();
                }
                std::lock_guard<std::mutex> lock(mu);
                done = true;
                cv.notify_one();
            });
        std::unique_lock<std::mutex> lock(mu);
        cv.wait(lock, [&done] { return done; });
    }

  private:
    std::unique_ptr<caspaxos::Server::Stub> stub;
};

int main(int argc, char* argv[]) {
    absl::ParseCommandLine(argc, argv);
    absl::SetStderrThreshold(absl::LogSeverity::kInfo);
    absl::InitializeLog();
    std::string target_str = absl::GetFlag(FLAGS_target);

    std::optional<int32_t> value = absl::GetFlag(FLAGS_value);

    Client client(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    client.Change(value);

    return 0;
}
