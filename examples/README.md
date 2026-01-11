# Examples

Example clients demonstrating how to interact with the BIT-S protocol.

## C++ Example Client

Build and run:
```bash
cd examples
g++ -std=c++17 example_client.cpp -o example_client \
    -I../cpp_vision/src/generated \
    -I../cpp_vision/src \
    -lgrpc++ -lprotobuf \
    -L../cpp_vision/build
./example_client
```

## Rust Example Client

Build and run:
```bash
cd examples
# First, ensure proto bindings are generated
cd ../rust_orchestrator && cargo build
cd ../examples
cargo run --bin example_client
```

## Usage

1. Start the C++ Vision Node:
   ```bash
   cd cpp_vision/build && ./cpp_vision
   ```

2. In another terminal, run the example client:
   ```bash
   # C++ client
   ./example_client
   
   # Or Rust client
   cargo run --bin example_client
   ```

## What the Examples Do

Both examples:
1. Create a sample quantized latent embedding
2. Send an `OP_INFER_REQ` frame to the Vision Node
3. Receive and display the inference response
4. Demonstrate the basic request/response pattern
