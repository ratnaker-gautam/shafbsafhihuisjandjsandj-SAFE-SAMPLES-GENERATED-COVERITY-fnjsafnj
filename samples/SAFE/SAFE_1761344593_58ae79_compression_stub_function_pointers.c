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
        
        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }
        
        output[out_idx++] = count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i + 1 < input_size && out_idx < output_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) {
            break;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }
    
    return out_idx;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    uint8_t input_buffer[MAX_INPUT_SIZE];
    uint8_t compressed_buffer[MAX_OUTPUT_SIZE];
    uint8_t decompressed_buffer[MAX_INPUT_SIZE];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return;
    }
    
    size_t input_len = strlen((char*)input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return;
    }
    
    size_t compressed_size = compress(input_buffer, input_len, compressed_buffer, sizeof(compressed_buffer));
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed_size);
    
    if (input_len > 0) {
        double ratio = (double)compressed_size / input_len * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    size_t decompressed_size = decompress(compressed_buffer, compressed_size, decompressed_buffer, sizeof(decompressed_buffer));
    
    if (decompressed_size != input_len) {
        printf("Decompression verification failed\n");
        return;
    }
    
    if (memcmp(input_buffer, decompressed_buffer, input_len) != 0) {
        printf("Decompression data mismatch\n");
        return;
    }
    
    printf("Decompression verified successfully\n");
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed_size && i < 32; i++) {
        printf("%02x ", compressed_buffer[i]);
    }
    if (compressed_size > 32) {
        printf("...");
    }
    printf("\n");
}

int main(void) {
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    printf("Run-Length Encoding Compression Demo\n");
    printf("====================================\n");
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}