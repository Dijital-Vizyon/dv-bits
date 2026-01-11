# dv-bits | BIT-S: Binary Inter-agent Transport Schema

**High-speed, low-latency binary communication protocol for autonomous AI agents.**

BIT-S is a protocol designed to bypass the overhead of human-readable formats (JSON/YAML) by utilizing **Quantized Latent Embeddings** and **Binary Opcodes**. It enables sub-millisecond communication between C++-based Vision engines and Rust-based logic orchestrators.

## 🚀 Key Features

* **Zero-Copy Serialization:** Built on Protobuf/gRPC for direct memory-to-buffer mapping.
* **Quantized Latent Space:** Transmit "thoughts" as `int8` vectors to save 75% bandwidth vs `float32`.
* **Multi-Language:** First-class support for **C++** (Inference) and **Rust** (Safety & Orchestration).
* **Automated Tooling:** GitHub Actions pipeline for instant SDK generation.

---

## 🏗 Project Structure

```text
├── proto/               # Single source of truth (AI Transport Schema)
├── cpp_vision/          # C++ Vision Node (High-performance inference)
├── rust_orchestrator/   # Rust Agent (Logic & Safety)
├── .github/workflows/   # CI/CD for auto-generating bindings
└── docker-compose.yml   # Local development mesh

```

---

## 🛠 Getting Started

### Prerequisites

* Docker & Docker Compose
* Protobuf Compiler (`protoc`)

### Local Simulation

To spin up the C++ Vision Node and the Rust Orchestrator in a local network mesh:

```bash
docker-compose up --build

```

The Orchestrator will begin sending `OP_INFER_REQ` frames to the Vision Node at `localhost:50051`.

---

## 📡 The Protocol (Protobuf)

The core communication unit is the `AgentFrame`. It encapsulates the intent (OpCode) and the data (LatentSpace).

```proto
message LatentSpace {
    uint32 dimensions = 1;
    bytes quantized_vector = 2; // int8 quantized data
    float scale_factor = 3;     // Dequantization constant
}

```

---

## 🤖 CI/CD Integration

This repo uses GitHub Actions to ensure that your C++ and Rust codebases never drift from the protocol definition.

1. Modify `proto/ai_transport.proto`.
2. Push to `main`.
3. The workflow automatically regenerates the headers and crates.

---

## ⚖️ License

This project is licensed under the **MIT License**. See the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.

---

## 🔗 Links

* **Repository:** [Dijital-Vizyon/dv-bits](https://github.com/Dijital-Vizyon/dv-bits)
* **Company:** [Digital Vision](https://dv.com.tr)
* **Maintainer:** [Mehmet T. AKALIN](https://dv.com.tr/makalin/)
* **LinkedIn:** [makalin](https://www.linkedin.com/in/makalin/)
