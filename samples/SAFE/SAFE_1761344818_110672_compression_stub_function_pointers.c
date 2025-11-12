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
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; i++) {
        if (out_idx + 2 >= max_output_size) {
            free(output.data);
            output.data = NULL;
            return output;
        }
        
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + 1 < input->size && input->data[i + 1] == current && count < 255) {
            count++;
            i++;
        }
        
        output.data[out_idx++] = (uint8_t)count;
        output.data[out_idx++] = current;
    }
    
    output.size = out_idx;
    return output;
}

Buffer decompress_simple(const Buffer *input) {
    Buffer output = {NULL, 0};
    if (input == NULL || input->data == NULL || input->size == 0 || input->size % 2 != 0) {
        return output;
    }
    
    size_t estimated_size = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        if (input->data[i] == 0) {
            return output;
        }
        if (estimated_size + input->data[i] < estimated_size) {
            return output;
        }
        estimated_size += input->data[i];
    }
    
    if (estimated_size > MAX_INPUT_SIZE) {
        return output;
    }
    
    output.data = malloc(estimated_size);
    if (output.data == NULL) {
        return output;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        for (uint8_t j = 0; j < count; j++) {
            if (out_idx >= estimated_size) {
                free(output.data);
                output.data = NULL;
                return output;
            }
            output.data[out_idx++] = value;
        }
    }
    
    output.size = out_idx;
    return output;
}

void process_compression(CompressionFunc func, const Buffer *input) {
    if (func == NULL || input == NULL) {
        printf("Invalid parameters\n");
        return;
    }
    
    Buffer result = func(input);
    if (result.data == NULL) {
        printf("Operation failed\n");
        return;
    }
    
    printf("Result size: %zu bytes\n", result.size);
    printf("First 16 bytes: ");
    for (size_t i = 0; i < result.size && i < 16; i++) {
        printf("%02X ", result.data[i]);
    }
    printf("\n");
    
    free(result.data);
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer input = {(uint8_t*)input_str, len};
    
    printf("\nCompressing...\n");
    process_compression(compress_simple, &input);
    
    Buffer compressed = compress_simple(&input);
    if (compressed.data != NULL) {
        printf("\nDecompressing...\n");
        process_compression(decompress_simple, &compressed);
        free(compressed.data);
    }
    
    return 0;
}