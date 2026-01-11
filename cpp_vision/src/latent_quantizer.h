#ifndef LATENT_QUANTIZER_H
#define LATENT_QUANTIZER_H

#include <vector>
#include <cstdint>
#include <cstddef>

struct QuantizedData {
    std::vector<int8_t> data;
    uint32_t dimensions;
    float scale_factor;
    float zero_point;
};

class LatentQuantizer {
public:
    // Quantize float32 vector to int8 (75% bandwidth reduction)
    QuantizedData Quantize(const std::vector<float>& input);
    
    // Dequantize int8 vector back to float32
    std::vector<float> Dequantize(const std::string& quantized_bytes,
                                  float scale_factor,
                                  float zero_point,
                                  uint32_t dimensions);
    
private:
    float FindScaleFactor(const std::vector<float>& input);
    float FindZeroPoint(const std::vector<float>& input, float scale);
};

#endif // LATENT_QUANTIZER_H
