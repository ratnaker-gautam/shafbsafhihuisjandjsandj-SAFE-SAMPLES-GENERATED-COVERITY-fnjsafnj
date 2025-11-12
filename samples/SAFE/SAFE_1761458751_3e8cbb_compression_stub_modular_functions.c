//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    char input[MAX_INPUT_SIZE + 1];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return buffer;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return buffer;
    }
    
    buffer.data = malloc(len);
    if (buffer.data == NULL) {
        return buffer;
    }
    
    memcpy(buffer.data, input, len);
    buffer.size = len;
    return buffer;
}

int validate_buffer(const Buffer* buffer) {
    if (buffer == NULL || buffer->data == NULL || buffer->size == 0) {
        return 0;
    }
    if (buffer->size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

Buffer compress_rle(const Buffer* input) {
    Buffer output = {NULL, 0};
    
    if (!validate_buffer(input)) {
        return output;
    }
    
    uint8_t* compressed = malloc(MAX_OUTPUT_SIZE);
    if (compressed == NULL) {
        return output;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input->size && out_index < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && 
               input->data[i + count] == current) {
            count++;
        }
        
        if (count >= 3 || current == 0xFF) {
            compressed[out_index++] = 0xFF;
            compressed[out_index++] = current;
            compressed[out_index++] = (uint8_t)count;
        } else {
            for (size_t j = 0; j < count && out_index < MAX_OUTPUT_SIZE; j++) {
                compressed[out_index++] = current;
            }
        }
        
        i += count;
    }
    
    if (out_index > 0) {
        output.data = malloc(out_index);
        if (output.data != NULL) {
            memcpy(output.data, compressed, out_index);
            output.size = out_index;
        }
    }
    
    free(compressed);
    return output;
}

void display_compression_info(const Buffer* input, const Buffer* output) {
    if (!validate_buffer(input) || !validate_buffer(output)) {
        printf("Invalid buffers for display\n");
        return;
    }
    
    printf("Original size: %zu bytes\n", input->size);
    printf("Compressed size: %zu bytes\n", output->size);
    
    if (input->size > 0) {
        double ratio = (double)output->size / input->size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output->size && i < 32; i++) {
        printf("%02X ", output->data[i]);
    }
    if (output->size > 32) {
        printf("...");
    }
    printf("\n");
}

void cleanup_buffer(Buffer* buffer) {
    if (buffer != NULL && buffer->data != NULL) {
        free(buffer->data);
        buffer->data = NULL;
        buffer->size = 0;
    }
}

int main(void) {
    Buffer input = read_input();
    if (!validate_buffer(&input)) {
        printf("No valid input received\n");
        return 1;
    }
    
    Buffer compressed = compress_rle(&input);
    if (!validate_buffer(&compressed)) {
        printf("Compression failed\n");
        cleanup_buffer(&input);
        return 1;
    }
    
    display_compression_info(&input, &compressed);
    
    cleanup_buffer(&input);
    cleanup_buffer(&compressed);
    
    return 0;
}