FROM ubuntu:22.04

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    autoconf \
    libtool \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Install Protobuf
RUN cd /tmp && \
    git clone --depth=1 --branch=v25.0 https://github.com/protocolbuffers/protobuf.git && \
    cd protobuf && \
    git submodule update --init --recursive && \
    ./autogen.sh && \
    ./configure && \
    make -j$(nproc) && \
    make install && \
    ldconfig && \
    cd / && rm -rf /tmp/protobuf

# Install gRPC
RUN cd /tmp && \
    git clone --recurse-submodules -b v1.60.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc && \
    cd grpc && \
    mkdir -p cmake/build && \
    cd cmake/build && \
    cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF ../.. && \
    make -j$(nproc) && \
    make install && \
    ldconfig && \
    cd / && rm -rf /tmp/grpc

# Set working directory
WORKDIR /app

# Copy proto files
COPY proto/ /app/proto/

# Copy C++ source
COPY cpp_vision/ /app/cpp_vision/

# Build
WORKDIR /app/cpp_vision
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make -j$(nproc)

# Run
WORKDIR /app/cpp_vision/build
CMD ["./cpp_vision"]
