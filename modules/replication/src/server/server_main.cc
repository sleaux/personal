#include <iostream>
#include <string>

#include "absl/base/log_severity.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

using grpc::CallbackServerContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

int32_t value;

class ServerImpl final : public caspaxos::Server::CallbackService {
    grpc::ServerUnaryReactor* Change(
        CallbackServerContext* context, const caspaxos::ChangeRequest* request,
        caspaxos::ChangeResponse* response) override {
        class Reactor : public grpc::ServerUnaryReactor {
            // NOTE The callback methods (e.g., OnDone()) are supposed to return
            // quickly. Never perform blocking work (e.g., waiting for an event)
            // in such callbacks.
          public:
            Reactor(const caspaxos::ChangeRequest& request,
                    caspaxos::ChangeResponse& response) {
                if (request.has_value()) {
                    // apply the change function to update with new value
                    value = request.value();
                } else {
                    // apply the null change function
                }
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

        return new Reactor(*request, *response);
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
};

void RunServer(uint16_t port) {
    std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
    ServerImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char* argv[]) {
    absl::ParseCommandLine(argc, argv);
    absl::SetStderrThreshold(absl::LogSeverity::kInfo);
    absl::InitializeLog();

    RunServer(absl::GetFlag(FLAGS_port));
    return 0;
}
