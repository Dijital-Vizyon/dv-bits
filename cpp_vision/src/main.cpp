#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "vision_service.h"
#include "ai_transport.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    VisionServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "C++ Vision Node listening on " << server_address << std::endl;
    std::cout << "Ready to process inference requests..." << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    std::cout << "BIT-S C++ Vision Node v1.0.0" << std::endl;
    std::cout << "High-performance inference engine" << std::endl;
    std::cout << "=================================" << std::endl;
    
    RunServer();
    return 0;
}
