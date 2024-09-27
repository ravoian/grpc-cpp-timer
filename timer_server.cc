#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

#include <grpc++/grpc++.h>
#include "timer.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using timer::TimeRequest;
using timer::TimeReply;
using timer::Timer;

std::string getCurrentTime() {
    // Get the current time as a time_point
    auto now = std::chrono::system_clock::now();
    
    // Convert it to a time_t for easier formatting
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    
    // Create a tm structure from the time_t
    std::tm time_info;
    localtime_s(&time_info, &now_time);  // Use localtime_s on Windows; localtime on Linux/macOS
    
    // Format the time into a string
    std::ostringstream time_stream;
    time_stream << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S");
    
    return time_stream.str();
}

// Logic and data behind the server's behavior.
class TimerServiceImpl final : public Timer::Service {
    Status SayHello(ServerContext * context, const TimeRequest * request, TimeReply * reply) override {
        std::string prefix("Hello ");
        std::string time = getCurrentTime();
        reply->set_message(prefix + request->name() + "! " + "The current date & time is: " + time);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    TimerServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with clients. 
    // In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. 
    // Note that some other thread must be responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char ** argv) {
    RunServer();
    return 0;
}

