#ifndef VISION_SERVICE_H
#define VISION_SERVICE_H

#include <grpcpp/grpcpp.h>
#include "ai_transport.grpc.pb.h"
#include "latent_quantizer.h"

using grpc::ServerContext;
using grpc::Status;
using bits::AgentFrame;
using bits::AgentTransport;

class VisionServiceImpl final : public AgentTransport::Service {
public:
    Status SendFrame(ServerContext* context, const AgentFrame* request, AgentFrame* response) override;
    Status StreamFrames(ServerContext* context, 
                       grpc::ServerReaderWriter<AgentFrame, AgentFrame>* stream) override;
    Status HealthCheck(ServerContext* context, const AgentFrame* request, AgentFrame* response) override;

private:
    LatentQuantizer quantizer_;
    uint32_t sequence_counter_ = 0;
    
    AgentFrame ProcessInferenceRequest(const AgentFrame& request);
    void SetFrameMetadata(AgentFrame* frame, const std::string& agent_id);
};

#endif // VISION_SERVICE_H
