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
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_idx++] = count;
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

int validate_buffer(const Buffer *buf) {
    return buf != NULL && buf->data != NULL && buf->size > 0 && buf->size <= MAX_INPUT_SIZE;
}

void process_compression(CompressFunc compress, DecompressFunc decompress) {
    Buffer input_buf = {0};
    Buffer output_buf = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char input_text[MAX_INPUT_SIZE];
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return;
    }
    
    input_buf.data = (uint8_t*)input_text;
    input_buf.size = input_len;
    
    if (!validate_buffer(&input_buf)) {
        printf("Invalid input buffer\n");
        return;
    }
    
    uint8_t compressed[MAX_OUTPUT_SIZE];
    size_t compressed_size = compress(input_buf.data, input_buf.size, compressed, sizeof(compressed));
    
    if (compressed_size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Compressed %zu bytes to %zu bytes\n", input_buf.size, compressed_size);
    
    uint8_t decompressed[MAX_INPUT_SIZE];
    size_t decompressed_size = decompress(compressed, compressed_size, decompressed, sizeof(decompressed));
    
    if (decompressed_size == 0) {
        printf("Decompression failed\n");
        return;
    }
    
    if (decompressed_size != input_buf.size || memcmp(input_buf.data, decompressed, input_buf.size) != 0) {
        printf("Round-trip verification failed\n");
        return;
    }
    
    printf("Original: %s\n", input_buf.data);
    printf("Decompressed: ");
    for (size_t i = 0; i < decompressed_size; i++) {
        putchar(decompressed[i]);
    }
    printf("\n");
    
    double ratio = (double)compressed_size / input_buf.size * 100.0;
    printf("Compression ratio: %.1f%%\n", ratio);
}

int main() {
    CompressFunc compress_funcs[] = {run_length_encode};
    DecompressFunc decompress_funcs[] = {run_length_decode};
    
    printf("Compression Stub Demo\n");
    printf("====================\n");
    
    for (size_t i = 0; i < sizeof(compress_funcs) / sizeof(compress_funcs[0]); i++) {
        printf("\nAlgorithm %zu:\n", i + 1);
        process_compression(compress_funcs[i], decompress_funcs[i]);
    }
    
    return 0;
}