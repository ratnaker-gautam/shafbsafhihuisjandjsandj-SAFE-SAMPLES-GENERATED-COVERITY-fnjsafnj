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
    if (output_size < input_size * 2) return 0;
    
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
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size && out_idx < output_size) {
        if (i + 1 >= input_size) break;
        
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (out_idx + count > output_size) break;
        
        for (uint8_t j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
        
        i += 2;
    }
    
    return out_idx;
}

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    char user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return;
    }
    
    input.data = (uint8_t*)user_input;
    input.size = len;
    
    if (!validate_buffer(&input)) {
        printf("Invalid input buffer\n");
        return;
    }
    
    uint8_t comp_buf[MAX_OUTPUT_SIZE];
    compressed.data = comp_buf;
    compressed.size = compress(input.data, input.size, comp_buf, sizeof(comp_buf));
    
    if (compressed.size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    uint8_t decomp_buf[MAX_INPUT_SIZE];
    decompressed.data = decomp_buf;
    decompressed.size = decompress(compressed.data, compressed.size, decomp_buf, sizeof(decomp_buf));
    
    if (decompressed.size == 0) {
        printf("Decompression failed\n");
        return;
    }
    
    printf("Original size: %zu bytes\n", input.size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input.size) * 100);
    
    if (decompressed.size == input.size && memcmp(input.data, decompressed.data, input.size) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip verification failed\n");
    }
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 32; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
}

int main() {
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}