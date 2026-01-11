# Contributing to BIT-S

Thank you for your interest in contributing to BIT-S!

## Development Setup

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Dijital-Vizyon/dv-bits.git
   cd dv-bits
   ```

2. **Install dependencies:**
   - Protobuf compiler (`protoc`)
   - gRPC libraries
   - CMake (for C++)
   - Rust toolchain (for Rust)

3. **Generate bindings:**
   ```bash
   make proto
   ```

4. **Build:**
   ```bash
   make all
   ```

## Development Workflow

1. **Modify the protocol:**
   - Edit `proto/ai_transport.proto`
   - Run `make proto` to regenerate bindings
   - Update implementations in C++ and Rust

2. **Test locally:**
   ```bash
   make docker-up
   # Test your changes
   make docker-down
   ```

3. **Submit a PR:**
   - Create a feature branch
   - Make your changes
   - Ensure CI passes
   - Submit pull request

## Code Style

- **C++**: Follow Google C++ Style Guide
- **Rust**: Use `rustfmt` and `clippy`
- **Protobuf**: Use snake_case for fields

## Testing

- Run Rust tests: `make test`
- Test Docker setup: `make docker-up`
- Check logs: `docker-compose logs`

## Questions?

Feel free to open an issue or contact the maintainer.
