use crate::quantizer::QuantizedData;
use anyhow::{Context, Result};
use bits::agent_transport_client::AgentTransportClient;
use bits::{AgentFrame, FrameMetadata, LatentSpace, OpCode};
use std::time::{SystemTime, UNIX_EPOCH};
use tonic::Request;

pub mod bits {
    tonic::include_proto!("bits");
}

pub struct VisionClient {
    client: AgentTransportClient<tonic::transport::Channel>,
}

impl VisionClient {
    pub async fn new(endpoint: &str) -> Result<Self> {
        let client = AgentTransportClient::connect(endpoint.to_string())
            .await
            .context("Failed to connect to vision node")?;
        
        Ok(Self { client })
    }
    
    pub async fn send_inference_request(
        &mut self,
        quantized: QuantizedData,
        sequence: u32,
    ) -> Result<QuantizedData> {
        let mut frame = AgentFrame {
            opcode: OpCode::OpInferReq as i32,
            latent: Some(LatentSpace {
                dimensions: quantized.dimensions,
                quantized_vector: quantized.data,
                scale_factor: quantized.scale_factor,
                zero_point: quantized.zero_point,
            }),
            metadata: Some(create_metadata("rust_orchestrator", sequence)),
            raw_data: vec![],
            error_message: String::new(),
        };
        
        let request = Request::new(frame);
        let response = self
            .client
            .send_frame(request)
            .await
            .context("Failed to send frame")?
            .into_inner();
        
        if response.opcode == OpCode::OpError as i32 {
            anyhow::bail!("Vision node returned error: {}", response.error_message);
        }
        
        let latent = response
            .latent
            .context("Response missing latent space")?;
        
        Ok(QuantizedData {
            dimensions: latent.dimensions,
            data: latent.quantized_vector,
            scale_factor: latent.scale_factor,
            zero_point: latent.zero_point,
        })
    }
    
    pub async fn health_check(&mut self) -> Result<()> {
        let frame = AgentFrame {
            opcode: OpCode::OpHeartbeat as i32,
            latent: None,
            metadata: Some(create_metadata("rust_orchestrator", 0)),
            raw_data: vec![],
            error_message: String::new(),
        };
        
        let request = Request::new(frame);
        self.client
            .health_check(request)
            .await
            .context("Health check failed")?;
        
        Ok(())
    }
}

fn create_metadata(agent_id: &str, sequence: u32) -> FrameMetadata {
    let timestamp = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_nanos() as u64;
    
    FrameMetadata {
        timestamp_ns: timestamp,
        agent_id: agent_id.to_string(),
        sequence_number: sequence,
        context: std::collections::HashMap::new(),
    }
}
