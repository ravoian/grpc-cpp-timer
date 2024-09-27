#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "timer.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using timer::TimeRequest;
using timer::TimeReply;
using timer::Timer;

class TimerClient {
public:
    // Constructor
    TimerClient(std::shared_ptr<Channel> channel): stub_(Timer::NewStub(channel)) {}
    // Assembles the client's payload, sends it and presents the response back from the server.
    std::string SayHello(const std::string & user) {
        // Data we are sending to the server.
        TimeRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        TimeReply reply;

        // Context for the client. 
        // It could be used to convey extra information to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->SayHello(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.message();
        } 
        else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return "gRPC failed";
        }
    }

private:
    std::unique_ptr<Timer::Stub> stub_;
};

void InterativeGRPC() {
    // Instantiate the client. It requires a channel, out of which the actual RPCs are created. 
    // This channel models a connection to an endpoint (in this case, localhost at port 50051). 
    // We indicate that the channel isn't authenticated (use of InsecureChannelCredentials()).
    TimerClient timer(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    while (true) {
        std::string user;
        std::cout << "Please enter your name:" << std::endl;
        // std::cin >> user;
        std::getline(std::cin, user);
        std::string reply = timer.SayHello(user);
        if (reply == "gRPC failed") {
            std::cout << "gRPC failed" << std::endl;
        }
        std::cout << "gRPC returned: " << std::endl;
        std::cout << reply << std::endl;
    }
}


int main() {

    InterativeGRPC();

    return 0;
}



