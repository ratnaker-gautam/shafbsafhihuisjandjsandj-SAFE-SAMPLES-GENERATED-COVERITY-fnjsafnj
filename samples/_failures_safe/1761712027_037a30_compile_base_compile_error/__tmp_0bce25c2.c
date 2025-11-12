//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    
    if (input_size > output_size) return 0;
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return null_compress(input, input_size, output, output_size);
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

void process_compression(CompressFunc compress, DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (!validate_buffer(input) || output == NULL) return;
    
    uint8_t temp_output[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress(input->data, input->size, temp_output, MAX_OUTPUT_SIZE);
    
    if (compressed_size == 0 || compressed_size > MAX_OUTPUT_SIZE) {
        printf("Compression failed\n");
        return;
    }
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress(temp_output, compressed_size, decompressed, MAX_INPUT_SIZE);
    
    if (decompressed_size != input->size || memcmp(input->data, decompressed, input->size) != 0) {
        printf("Round-trip verification failed\n");
        return;
    }
    
    double ratio = (double)compressed_size / input->size * 100.0;
    printf("Compressed %zu bytes to %zu bytes (%.1f%%)\n", input->size, compressed_size, ratio);
    
    if (output->size >= compressed_size) {
        memcpy(output->data, temp_output, compressed_size);
        output->size = compressed_size;
        printf("Output buffer updated with compressed data\n");
    } else {
        printf("Output buffer too small\n");
    }
}

int main() {
    Buffer input_buf, output_buf;
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t output_data[MAX_OUTPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen((char*)input_data);
    if (input_len > 0 && input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    input_buf.data = input_data;
    input_buf.size = input_len;
    output_buf.data = output_data;
    output_buf.size = MAX_OUTPUT_SIZE;
    
    CompressFunc compressors[] = {run_length_encode, null_compress};
    DecompressFunc decompressors[] = {run_length_decode, null_decompress};
    const char *names[] = {"Run-length encoding", "Null compression"};
    size_t num_methods = sizeof(compressors) / sizeof(compress