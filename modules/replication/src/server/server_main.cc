#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

#include "absl/base/log_severity.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "grpc++/grpc++.h"
#include "modules/replication/proto/caspaxos.grpc.pb.h"

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");
ABSL_FLAG(std::vector<std::string>, node, {}, "URIs of other server nodes");

using grpc::CallbackServerContext;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

int32_t value;

class ServerImpl final : public caspaxos::Server::CallbackService {
  public:
    ServerImpl(const std::vector<std::string> peer_uris) {
        for (const auto uri : peer_uris) {
            auto channel =
                grpc::CreateChannel(uri, grpc::InsecureChannelCredentials());
            stubs.emplace_back(caspaxos::Server::NewStub(channel));
        }
    }

  private:
    grpc::ServerUnaryReactor* Change(
        CallbackServerContext* context, const caspaxos::ChangeRequest* request,
        caspaxos::ChangeResponse* response) override {
        class Reactor : public grpc::ServerUnaryReactor {
            // NOTE The callback methods (e.g., OnDone()) are supposed to return
            // quickly. Never perform blocking work (e.g., waiting for an event)
            // in such callbacks.
          public:
            Reactor(const caspaxos::ChangeRequest& request,
                    caspaxos::ChangeResponse& response,
                    const std::vector<std::unique_ptr<caspaxos::Server::Stub>>&
                        stubs) {
                //  state Prepare
                //    entry
                //      send out prepares
                //    on ePrepareResponse do
                //      if quorum, goto Accept
                //      if not, return unmodified state

                //  state Accept
                //    entry
                //      apply change function to this node's state
                //      send out accept requests
                //    on AcceptResponse
                //      if quorum, return modified state
                //      if not, return unmodified state

                for (const auto& stub : stubs) {
                    ClientContext context;
                    caspaxos::PrepareRequest request;
                    caspaxos::PrepareResponse response;
                    std::mutex mu;
                    std::condition_variable cv;
                    bool done = false;
                    stub->async()->Prepare(
                        &context, &request, &response,
                        [&mu, &cv, &done, &response, this](Status status) {
                            bool ret;
                            if (!status.ok()) {
                                LOG(INFO) << "Prepare rpc failed.";
                            } else {
                                LOG(INFO) << "Prepare rpc ok.";
                            }
                            std::lock_guard<std::mutex> lock(mu);
                            done = true;
                            cv.notify_one();
                        });
                    std::unique_lock<std::mutex> lock(mu);
                    cv.wait(lock, [&done] { return done; });
                }
                // self-prepare

                response.set_value(value);
                Finish(grpc::Status::OK);
            }

          private:
            void OnDone() override {
                LOG(INFO) << "Change done.";
                delete this;
            }

            void OnCancel() override { LOG(ERROR) << "Change cancelled"; }
        };

        return new Reactor(*request, *response, stubs);
    }

    grpc::ServerUnaryReactor* Prepare(
        CallbackServerContext* context, const caspaxos::PrepareRequest* request,
        caspaxos::PrepareResponse* response) override {
        class Reactor : public grpc::ServerUnaryReactor {
            // NOTE The callback methods (e.g., OnDone()) are supposed to return
            // quickly. Never perform blocking work (e.g., waiting for an event)
            // in such callbacks.
          public:
            Reactor(const caspaxos::PrepareRequest& point) {
                Finish(grpc::Status::OK);
            }

          private:
            void OnDone() override {
                LOG(INFO) << "Prepare done.";
                delete this;
            }

            void OnCancel() override { LOG(ERROR) << "Prepare cancelled."; }
        };

        return new Reactor(*request);
    }

    grpc::ServerUnaryReactor* Accept(
        CallbackServerContext* context, const caspaxos::AcceptRequest* request,
        caspaxos::AcceptResponse* response) override {
        class Reactor : public grpc::ServerUnaryReactor {
            // NOTE The callback methods (e.g., OnDone()) are supposed to return
            // quickly. Never perform blocking work (e.g., waiting for an event)
            // in such callbacks.
          public:
            Reactor(const caspaxos::AcceptRequest& point) {
                Finish(grpc::Status::OK);
            }

          private:
            void OnDone() override {
                LOG(INFO) << "Accept done.";
                delete this;
            }

            void OnCancel() override { LOG(ERROR) << "Accept cancelled."; }
        };

        return new Reactor(*request);
    }

    std::vector<std::unique_ptr<caspaxos::Server::Stub>> stubs;
};

class Context;

class State {
  protected:
    Context* context;

  public:
    virtual ~State() {}

    void set_context(Context* context) { this->context = context; }

    virtual void Entry(){};
    virtual std::string AsString() = 0;
};

class Idle : public State {
    void Entry() override {}
    std::string AsString() override { return "Idle"; }
};

class Prepare : public State {
    void Entry() override {}
    std::string AsString() override { return "Prepare"; }
};

class Accept : public State {
    void Entry() override {}
    std::string AsString() override { return "Accept"; }
};

class Context {
  private:
    std::unique_ptr<State> state;

  public:
    Context(std::unique_ptr<State> state) : state(nullptr) {
        this->TransitionTo(std::move(state));
    }
    void TransitionTo(std::unique_ptr<State> state) {
        LOG(INFO) << "Context: Transition to " << state->AsString();
        this->state = std::move(state);
        this->state->set_context(this);
        this->state->Entry();
    }
};

class CasPaxosNode {
  public:
    CasPaxosNode() {
        ctx = std::make_unique<Context>(std::make_unique<Idle>());
    }

    void Start(uint16_t port) {
        std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
        ServerImpl service(absl::GetFlag(FLAGS_node));

        ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address,
                                 grpc::InsecureServerCredentials());
        // Register "service" as the instance through which we'll communicate
        // with clients. In this case it corresponds to an *synchronous*
        // service.
        builder.RegisterService(&service);
        // Finally assemble the server.
        server = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever
        // return.
        server->Wait();
    }

  private:
    std::unique_ptr<Context> ctx;
    std::unique_ptr<Server> server;
    int32_t timestamp;
    int32_t value;
};

int main(int argc, char* argv[]) {
    absl::ParseCommandLine(argc, argv);
    absl::SetStderrThreshold(absl::LogSeverity::kInfo);
    absl::InitializeLog();

    CasPaxosNode node;
    node.Start(absl::GetFlag(FLAGS_port));
    return 0;
}
