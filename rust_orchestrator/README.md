# Rust Orchestrator

Safety and logic orchestration engine for coordinating with the C++ Vision Node.

## Building

```bash
cargo build --release
```

## Running

```bash
# Default endpoint (localhost:50051)
cargo run

# Custom endpoint
VISION_ENDPOINT=http://localhost:50051 cargo run
```

## Features

- **Type-safe gRPC client**: Generated from Protobuf definitions
- **Quantization support**: Efficient int8 encoding/decoding
- **Error handling**: Robust error propagation with `anyhow`
- **Async runtime**: Built on Tokio for high concurrency

## Architecture

- **client.rs**: gRPC client wrapper for Vision Node communication
- **quantizer.rs**: Quantization/dequantization utilities
- **main.rs**: Main orchestration loop

## Safety Features

- Memory-safe Rust implementation
- Proper error handling and recovery
- Type-safe protocol bindings
