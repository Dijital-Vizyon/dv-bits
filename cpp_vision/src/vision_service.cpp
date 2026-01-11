#include "vision_service.h"
#include <chrono>
#include <iostream>
#include <random>

using bits::OpCode;
using bits::LatentSpace;
using bits::FrameMetadata;

Status VisionServiceImpl::SendFrame(ServerContext* context, 
                                    const AgentFrame* request, 
                                    AgentFrame* response) {
    std::cout << "[Vision] Received frame with opcode: " << request->opcode() << std::endl;
    
    switch (request->opcode()) {
        case OpCode::OP_INFER_REQ:
            *response = ProcessInferenceRequest(*request);
            break;
            
        case OpCode::OP_HEARTBEAT:
            response->set_opcode(OpCode::OP_HEARTBEAT);
            SetFrameMetadata(response, "cpp_vision");
            break;
            
        case OpCode::OP_SHUTDOWN:
            response->set_opcode(OpCode::OP_SHUTDOWN);
            response->set_error_message("Shutting down gracefully");
            return Status::OK;
            
        default:
            response->set_opcode(OpCode::OP_ERROR);
            response->set_error_message("Unknown opcode");
            return Status(grpc::StatusCode::INVALID_ARGUMENT, "Unknown opcode");
    }
    
    return Status::OK;
}

Status VisionServiceImpl::StreamFrames(ServerContext* context,
                                      grpc::ServerReaderWriter<AgentFrame, AgentFrame>* stream) {
    AgentFrame request;
    while (stream->Read(&request)) {
        AgentFrame response;
        
        if (request.opcode() == OpCode::OP_INFER_REQ) {
            response = ProcessInferenceRequest(request);
        } else if (request.opcode() == OpCode::OP_HEARTBEAT) {
            response.set_opcode(OpCode::OP_HEARTBEAT);
            SetFrameMetadata(&response, "cpp_vision");
        } else {
            response.set_opcode(OpCode::OP_ERROR);
            response.set_error_message("Unsupported opcode in stream");
        }
        
        if (!stream->Write(response)) {
            break;
        }
    }
    
    return Status::OK;
}

Status VisionServiceImpl::HealthCheck(ServerContext* context,
                                      const AgentFrame* request,
                                      AgentFrame* response) {
    response->set_opcode(OpCode::OP_HEARTBEAT);
    SetFrameMetadata(response, "cpp_vision");
    return Status::OK;
}

AgentFrame VisionServiceImpl::ProcessInferenceRequest(const AgentFrame& request) {
    AgentFrame response;
    response.set_opcode(OpCode::OP_INFER_RESP);
    
    // Dequantize input latent space
    if (request.has_latent()) {
        const auto& input_latent = request.latent();
        std::vector<float> dequantized = quantizer_.Dequantize(
            input_latent.quantized_vector(),
            input_latent.scale_factor(),
            input_latent.zero_point(),
            input_latent.dimensions()
        );
        
        // Simulate inference processing (in real implementation, this would run ML model)
        std::vector<float> output_features(dequantized.size());
        for (size_t i = 0; i < dequantized.size(); ++i) {
            // Simple transformation (replace with actual model inference)
            output_features[i] = dequantized[i] * 1.1f + 0.05f;
        }
        
        // Quantize output
        auto quantized = quantizer_.Quantize(output_features);
        
        LatentSpace* output_latent = response.mutable_latent();
        output_latent->set_dimensions(quantized.dimensions);
        output_latent->set_quantized_vector(quantized.data.data(), quantized.data.size());
        output_latent->set_scale_factor(quantized.scale_factor);
        output_latent->set_zero_point(quantized.zero_point);
    }
    
    SetFrameMetadata(&response, "cpp_vision");
    return response;
}

void VisionServiceImpl::SetFrameMetadata(AgentFrame* frame, const std::string& agent_id) {
    FrameMetadata* metadata = frame->mutable_metadata();
    auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    metadata->set_timestamp_ns(now);
    metadata->set_agent_id(agent_id);
    metadata->set_sequence_number(++sequence_counter_);
    metadata->mutable_context()->insert({"node_type", "vision"});
    metadata->mutable_context()->insert({"version", "1.0.0"});
}
