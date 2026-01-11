# Protocol Definitions

This directory contains the Protocol Buffer (Protobuf) definitions for the BIT-S protocol.

## Building

### Generate C++ bindings:
```bash
protoc --cpp_out=../cpp_vision/src/generated proto/ai_transport.proto --proto_path=.
protoc --grpc_out=../cpp_vision/src/generated --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` proto/ai_transport.proto --proto_path=.
```

### Generate Rust bindings:
```bash
protoc --rust_out=../rust_orchestrator/src/generated proto/ai_transport.proto --proto_path=.
```

## Protocol Overview

The `AgentFrame` is the core communication unit, containing:
- **OpCode**: The operation type (inference request, response, heartbeat, etc.)
- **LatentSpace**: Quantized embedding vector (int8 quantized for bandwidth efficiency)
- **FrameMetadata**: Timestamp, agent ID, sequence number, and context
- **raw_data**: Optional binary payload
- **error_message**: Error information if applicable
