#[derive(Debug, Clone)]
pub struct QuantizedData {
    pub data: Vec<u8>,
    pub dimensions: u32,
    pub scale_factor: f32,
    pub zero_point: f32,
}

pub struct LatentQuantizer;

impl LatentQuantizer {
    pub fn new() -> Self {
        Self
    }
    
    /// Quantize float32 vector to int8 (75% bandwidth reduction)
    pub fn quantize(&self, input: &[f32]) -> QuantizedData {
        if input.is_empty() {
            return QuantizedData {
                data: vec![],
                dimensions: 0,
                scale_factor: 1.0,
                zero_point: 0.0,
            };
        }
        
        let min_val = input.iter().copied().fold(f32::INFINITY, f32::min);
        let max_val = input.iter().copied().fold(f32::NEG_INFINITY, f32::max);
        
        let range = max_val - min_val;
        let scale = if range == 0.0 {
            1.0
        } else {
            range / 255.0
        };
        
        let zero_point = min_val;
        
        let quantized: Vec<u8> = input
            .iter()
            .map(|&value| {
                let quantized_value = ((value - zero_point) / scale).round() as i32;
                let clamped = quantized_value.clamp(-128, 127);
                (clamped as i8) as u8
            })
            .collect();
        
        QuantizedData {
            data: quantized,
            dimensions: input.len() as u32,
            scale_factor: scale,
            zero_point,
        }
    }
    
    /// Dequantize int8 vector back to float32
    pub fn dequantize(
        &self,
        quantized_bytes: &[u8],
        scale_factor: f32,
        zero_point: f32,
        dimensions: u32,
    ) -> Vec<f32> {
        quantized_bytes
            .iter()
            .take(dimensions as usize)
            .map(|&byte| {
                let signed = byte as i8;
                (signed as f32) * scale_factor + zero_point
            })
            .collect()
    }
}

impl Default for LatentQuantizer {
    fn default() -> Self {
        Self::new()
    }
}
