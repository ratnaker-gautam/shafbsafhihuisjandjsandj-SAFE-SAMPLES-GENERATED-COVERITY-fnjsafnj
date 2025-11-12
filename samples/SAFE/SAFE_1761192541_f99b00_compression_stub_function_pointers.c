//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
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

Buffer compress_simple(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0) {
        return output;
    }
    
    size_t max_output_size = input->size * COMPRESSED_MULTIPLIER;
    if (max_output_size < input->size) {
        return output;
    }
    
    output.data = malloc(max_output_size);
    if (output.data == NULL) {
        return output;
    }
    
    size_t out_pos = 0;
    for (size_t i = 0; i < input->size; i++) {
        if (out_pos + 2 > max_output_size) {
            free(output.data);
            output.data = NULL;
            return output;
        }
        
        output.data[out_pos++] = input->data[i];
        output.data[out_pos++] = 1;
    }
    
    output.size = out_pos;
    return output;
}

Buffer decompress_simple(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0 || input->size % 2 != 0) {
        return output;
    }
    
    output.size = input->size / 2;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    for (size_t i = 0, j = 0; i < input->size; i += 2, j++) {
        if (j >= output.size) {
            free(output.data);
            output.data = NULL;
            output.size = 0;
            return output;
        }
        output.data[j] = input->data[i];
    }
    
    return output;
}

void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02x", buf->data[i]);
    }
    printf("\n");
}

int main() {
    CompressionFunc compress_func = compress_simple;
    CompressionFunc decompress_func = decompress_simple;
    
    char input_str[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer input = {(uint8_t*)input_str, input_len};
    
    Buffer compressed = compress_func(&input);
    if (compressed.data == NULL) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed (%zu bytes): ", compressed.size);
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress_func(&compressed);
    if (decompressed.data == NULL) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        putchar(decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input.size && 
        memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
    
    return 0;
}