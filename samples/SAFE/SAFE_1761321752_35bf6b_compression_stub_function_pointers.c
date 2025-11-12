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
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output_size) return 0;
        
        output[out_idx++] = (uint8_t)count;
        output[out_idx++] = current;
        i += count;
    }
    
    return out_idx;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0) return 0;
    if (input_size % 2 != 0) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t count = input[i++];
        uint8_t value = input[i++];
        
        if (out_idx + count > output_size) return 0;
        
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
    
    printf("Enter input data (max %d bytes): ", MAX_INPUT_SIZE);
    
    char input_str[MAX_INPUT_SIZE + 1];
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return;
    }
    
    input_buf.data = (uint8_t*)input_str;
    input_buf.size = len;
    
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
    
    printf("Decompression successful: ");
    for (size_t i = 0; i < decompressed_size && i < 50; i++) {
        printf("%c", decompressed[i]);
    }
    if (decompressed_size > 50) printf("...");
    printf("\n");
}

int main() {
    CompressFunc compress_func = run_length_encode;
    DecompressFunc decompress_func = run_length_decode;
    
    process_compression(compress_func, decompress_func);
    
    return 0;
}