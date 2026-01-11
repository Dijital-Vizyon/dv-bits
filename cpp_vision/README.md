# C++ Vision Node

High-performance inference engine for processing quantized latent embeddings.

## Building

### Prerequisites
- CMake 3.15+
- C++17 compatible compiler
- Protobuf and gRPC libraries

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Docker Build

The Dockerfile in the root directory will build this automatically.

## Usage

```bash
./cpp_vision
```

The service will listen on `0.0.0.0:50051` for gRPC connections.

## Architecture

- **vision_service.cpp**: gRPC service implementation
- **latent_quantizer.cpp**: Quantization/dequantization utilities
- **main.cpp**: Entry point and server setup

## Performance

- Zero-copy serialization via Protobuf
- Quantized int8 embeddings (75% bandwidth reduction vs float32)
- Sub-millisecond latency for inference requests
