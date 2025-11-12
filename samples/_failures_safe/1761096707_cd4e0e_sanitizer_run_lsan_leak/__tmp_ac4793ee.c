//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef Buffer (*CompressionFunc)(const Buffer*);

Buffer null_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL) return output;
    
    size_t safe_size = input->size * COMPRESSED_MULTIPLIER;
    if (safe_size < input->size) return output;
    
    output.data = malloc(safe_size);
    if (output.data == NULL) return output;
    
    for (size_t i = 0; i < input->size; i++) {
        output.data[i] = input->data[i] ^ 0x55;
    }
    output.size = input->size;
    
    return output;
}

Buffer run_length_compress(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL) return output;
    
    size_t max_output_size = input->size * 2;
    if (max_output_size < input->size) return output;
    
    output.data = malloc(max_output_size);
    if (output.data == NULL) return output;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size;) {
        if (out_idx + 2 >= max_output_size) break;
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        output.data[out_idx++] = current;
        output.data[out_idx++] = (uint8_t)count;
        i += count;
    }
    
    output.size = out_idx;
    return output;
}

void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main() {
    CompressionFunc compressors[] = {null_compress, run_length_compress};
    const char *names[] = {"Null Compressor", "Run-Length Encoder"};
    int num_compressors = sizeof(compressors) / sizeof(compressors[0]);
    
    char input_str[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer input = {(uint8_t*)input_str, input_len};
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_buffer_hex(&input);
    
    for (int i = 0; i < num_compressors; i++) {
        printf("\n%s:\n", names[i]);
        Buffer compressed = compressors[i](&input);
        
        if (compressed.data != NULL && compressed.size > 0) {
            printf("Compressed data (%zu bytes):\n", compressed.size);
            print_buffer_hex(&compressed);
            double ratio = (double)compressed.size / (double)input.size * 100.0;
            printf("Compression ratio: %.1f%%\n", ratio);
            free(compressed.data);
        } else {
            printf("Compression failed\n");
        }
    }
    
    return 0;
}