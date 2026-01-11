#include "latent_quantizer.h"
#include <algorithm>
#include <cmath>
#include <limits>

QuantizedData LatentQuantizer::Quantize(const std::vector<float>& input) {
    if (input.empty()) {
        return {{}, 0, 1.0f, 0.0f};
    }
    
    float scale = FindScaleFactor(input);
    float zero_point = FindZeroPoint(input, scale);
    
    std::vector<int8_t> quantized;
    quantized.reserve(input.size());
    
    for (float value : input) {
        int32_t quantized_value = static_cast<int32_t>(
            std::round((value - zero_point) / scale)
        );
        // Clamp to int8 range
        quantized_value = std::max(-128, std::min(127, quantized_value));
        quantized.push_back(static_cast<int8_t>(quantized_value));
    }
    
    return {quantized, static_cast<uint32_t>(input.size()), scale, zero_point};
}

std::vector<float> LatentQuantizer::Dequantize(const std::string& quantized_bytes,
                                               float scale_factor,
                                               float zero_point,
                                               uint32_t dimensions) {
    std::vector<float> dequantized;
    dequantized.reserve(dimensions);
    
    const int8_t* data = reinterpret_cast<const int8_t*>(quantized_bytes.data());
    size_t size = std::min(static_cast<size_t>(dimensions), quantized_bytes.size());
    
    for (size_t i = 0; i < size; ++i) {
        float value = static_cast<float>(data[i]) * scale_factor + zero_point;
        dequantized.push_back(value);
    }
    
    return dequantized;
}

float LatentQuantizer::FindScaleFactor(const std::vector<float>& input) {
    auto [min_it, max_it] = std::minmax_element(input.begin(), input.end());
    float min_val = *min_it;
    float max_val = *max_it;
    
    float range = max_val - min_val;
    if (range == 0.0f) {
        return 1.0f;
    }
    
    // Scale to fit int8 range [-128, 127]
    return range / 255.0f;
}

float LatentQuantizer::FindZeroPoint(const std::vector<float>& input, float scale) {
    if (input.empty()) {
        return 0.0f;
    }
    
    float min_val = *std::min_element(input.begin(), input.end());
    return min_val;
}
