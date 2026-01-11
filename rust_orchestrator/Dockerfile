FROM rust:1.75 as builder

# Install protobuf compiler
RUN apt-get update && apt-get install -y \
    protobuf-compiler \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy proto files
COPY proto/ /app/proto/

# Copy Rust source
COPY rust_orchestrator/ /app/rust_orchestrator/

# Build
WORKDIR /app/rust_orchestrator
RUN cargo build --release

# Runtime stage
FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/rust_orchestrator/target/release/rust_orchestrator /app/

CMD ["/app/rust_orchestrator"]
