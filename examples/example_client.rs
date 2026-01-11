use bits::agent_transport_client::AgentTransportClient;
use bits::{AgentFrame, FrameMetadata, LatentSpace, OpCode};
use std::time::{SystemTime, UNIX_EPOCH};
use tonic::Request;

pub mod bits {
    tonic::include_proto!("bits");
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let endpoint = "http://[::1]:50051";
    let mut client = AgentTransportClient::connect(endpoint.to_string()).await?;
    
    println!("Sending inference request to {}", endpoint);
    
    // Create sample latent space
    let sample_features = vec![0.1, 0.2, 0.3, 0.4, 0.5];
    let quantized: Vec<u8> = sample_features
        .iter()
        .map(|&f| ((f * 127.0) as i8) as u8)
        .collect();
    
    let frame = AgentFrame {
        opcode: OpCode::OpInferReq as i32,
        latent: Some(LatentSpace {
            dimensions: sample_features.len() as u32,
            quantized_vector: quantized,
            scale_factor: 1.0 / 127.0,
            zero_point: 0.0,
        }),
        metadata: Some(FrameMetadata {
            timestamp_ns: SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .unwrap()
                .as_nanos() as u64,
            agent_id: "example_client".to_string(),
            sequence_number: 1,
            context: std::collections::HashMap::new(),
        }),
        raw_data: vec![],
        error_message: String::new(),
    };
    
    let request = Request::new(frame);
    let response = client.send_frame(request).await?;
    
    println!("Received response with opcode: {}", response.get_ref().opcode);
    if let Some(latent) = &response.get_ref().latent {
        println!("Response dimensions: {}", latent.dimensions);
    }
    
    Ok(())
}
