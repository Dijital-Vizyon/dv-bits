#include <iostream>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "../cpp_vision/src/generated/ai_transport.grpc.pb.h"
#include "../cpp_vision/src/latent_quantizer.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using bits::AgentFrame;
using bits::AgentTransport;
using bits::OpCode;
using bits::LatentSpace;
using bits::FrameMetadata;

class ExampleClient {
public:
    ExampleClient(std::shared_ptr<Channel> channel)
        : stub_(AgentTransport::NewStub(channel)) {}

    void SendInferenceRequest() {
        AgentFrame request;
        request.set_opcode(OpCode::OP_INFER_REQ);
        
        // Create sample latent space
        LatentQuantizer quantizer;
        std::vector<float> features = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};
        auto quantized = quantizer.Quantize(features);
        
        LatentSpace* latent = request.mutable_latent();
        latent->set_dimensions(quantized.dimensions);
        latent->set_quantized_vector(quantized.data.data(), quantized.data.size());
        latent->set_scale_factor(quantized.scale_factor);
        latent->set_zero_point(quantized.zero_point);
        
        // Set metadata
        FrameMetadata* metadata = request.mutable_metadata();
        metadata->set_agent_id("example_client");
        metadata->set_sequence_number(1);
        
        AgentFrame response;
        ClientContext context;
        
        Status status = stub_->SendFrame(&context, request, &response);
        
        if (status.ok()) {
            std::cout << "Received response with opcode: " << response.opcode() << std::endl;
            if (response.has_latent()) {
                const auto& resp_latent = response.latent();
                std::cout << "Response dimensions: " << resp_latent.dimensions() << std::endl;
            }
        } else {
            std::cout << "RPC failed: " << status.error_message() << std::endl;
        }
    }

private:
    std::unique_ptr<AgentTransport::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string server_address = "localhost:50051";
    
    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    ExampleClient client(channel);
    
    std::cout << "Sending inference request to " << server_address << std::endl;
    client.SendInferenceRequest();
    
    return 0;
}
