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
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx + 2 <= output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = current;
        output[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input_size && out_idx < output_size; i += 2) {
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > output_size) {
            return 0;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer input_buf = {0};
    Buffer output_buf = {0};
    
    uint8_t input_data[MAX_INPUT_SIZE];
    uint8_t compressed_data[MAX_OUTPUT_SIZE];
    uint8_t decompressed_data[MAX_INPUT_SIZE];
    
    printf("Enter input data (up to %d bytes as hex, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_size = 0;
    char *token = strtok(line, " \t\n");
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int value;
        if (sscanf(token, "%02x", &value) == 1 && value <= 0xFF) {
            input_data[input_size++] = (uint8_t)value;
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        printf("No valid input data provided\n");
        return;
    }
    
    input_buf.data = input_data;
    input_buf.size = input_size;
    
    if (!validate_buffer(&input_buf)) {
        printf("Invalid input buffer\n");
        return;
    }
    
    size_t compressed_size = compress(input_buf.data, input_buf.size, compressed_data, sizeof(compressed_data));
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed data (%zu bytes): ", compressed_size);
    for (size_t i = 0; i < compressed_size; i++) {
        printf("%02x ", compressed_data[i]);
    }
    printf("\n");
    
    output_buf.data = compressed_data;
    output_buf.size = compressed_size;
    
    if (!validate_buffer(&output_buf)) {
        printf("Invalid output buffer\n");
        return;
    }
    
    size_t decompressed_size = decompress(output_buf.data, output_buf.size, decompressed_data, sizeof(decompressed_data));
    
    if (decompressed_size == 0 || decompressed_size != input_size) {
        printf("Decompression failed\n");
        return;
    }
    
    if (memcmp(input_data, decompressed_data, input_size) != 0) {
        printf("Data mismatch after decompression\n");
        return;
    }
    
    printf("Decompression successful - data verified\n");
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed_size / input_size) * 100.0);
}

int main() {
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Run-Length Encoding Compression Demo\n");
    process_compression(compress_func, decompress_func);
    
    return 0;
}