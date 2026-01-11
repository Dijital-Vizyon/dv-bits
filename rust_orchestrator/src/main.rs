mod client;
mod quantizer;

use anyhow::Result;
use client::VisionClient;
use quantizer::LatentQuantizer;
use std::time::Duration;
use tokio::time::sleep;
use tracing::{info, error};

#[tokio::main]
async fn main() -> Result<()> {
    tracing_subscriber::fmt::init();
    
    info!("BIT-S Rust Orchestrator v1.0.0");
    info!("Safety & Logic Orchestration Engine");
    info!("====================================");
    
    let vision_endpoint = std::env::var("VISION_ENDPOINT")
        .unwrap_or_else(|_| "http://[::1]:50051".to_string());
    
    info!("Connecting to Vision Node at: {}", vision_endpoint);
    
    let mut client = VisionClient::new(&vision_endpoint).await?;
    let quantizer = LatentQuantizer::new();
    
    // Send periodic inference requests
    let mut sequence = 0u32;
    loop {
        sequence += 1;
        
        // Create a sample latent embedding (in practice, this would come from your model)
        let sample_features = vec![0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0];
        let quantized = quantizer.quantize(&sample_features);
        
        info!("Sending inference request #{}", sequence);
        
        match client.send_inference_request(quantized, sequence).await {
            Ok(response) => {
                info!("Received inference response");
                info!("  Dimensions: {}", response.dimensions);
                info!("  Scale factor: {}", response.scale_factor);
                info!("  Data size: {} bytes", response.data.len());
                
                // Dequantize and process response
                let dequantized = quantizer.dequantize(
                    &response.data,
                    response.scale_factor,
                    response.zero_point,
                    response.dimensions,
                );
                
                info!("Dequantized features: {:?}", &dequantized[..std::cmp::min(5, dequantized.len())]);
            }
            Err(e) => {
                error!("Error sending inference request: {}", e);
            }
        }
        
        // Wait before next request
        sleep(Duration::from_secs(2)).await;
    }
}
