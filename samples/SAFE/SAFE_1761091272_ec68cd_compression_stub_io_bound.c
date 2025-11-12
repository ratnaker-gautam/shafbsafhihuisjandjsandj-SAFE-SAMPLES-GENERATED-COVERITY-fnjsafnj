//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576
#define CHUNK_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int read_input(Buffer *buf) {
    size_t total_read = 0;
    char chunk[CHUNK_SIZE];
    
    while (total_read < MAX_INPUT_SIZE) {
        printf("Enter data chunk (or empty line to finish): ");
        if (fgets(chunk, CHUNK_SIZE, stdin) == NULL) {
            return -1;
        }
        
        if (chunk[0] == '\n') {
            break;
        }
        
        size_t chunk_len = strlen(chunk);
        if (chunk[chunk_len - 1] == '\n') {
            chunk[chunk_len - 1] = '\0';
            chunk_len--;
        }
        
        if (total_read + chunk_len >= MAX_INPUT_SIZE) {
            printf("Input too large\n");
            return -1;
        }
        
        memcpy(buf->data + total_read, chunk, chunk_len);
        total_read += chunk_len;
    }
    
    buf->size = total_read;
    return 0;
}

void simple_compress(const Buffer *input, Buffer *output) {
    if (input->size == 0) {
        output->size = 0;
        return;
    }
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_INPUT_SIZE - 2) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == '\\') {
            if (out_idx + 3 >= MAX_INPUT_SIZE) break;
            output->data[out_idx++] = '\\';
            output->data[out_idx++] = (uint8_t)count;
            output->data[out_idx++] = current;
        } else {
            if (out_idx + count >= MAX_INPUT_SIZE) break;
            for (size_t i = 0; i < count; i++) {
                output->data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output->size = out_idx;
}

void display_compression_stats(const Buffer *input, const Buffer *output) {
    printf("Original size: %zu bytes\n", input->size);
    printf("Compressed size: %zu bytes\n", output->size);
    
    if (input->size > 0) {
        double ratio = (double)output->size / input->size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
}

void display_compressed_data(const Buffer *output) {
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output->size && i < 64; i++) {
        printf("%02X ", output->data[i]);
    }
    if (output->size > 64) {
        printf("...");
    }
    printf("\n");
}

int main(void) {
    Buffer input_data = {0};
    Buffer compressed_data = {0};
    
    printf("Simple Run-Length Compression Tool\n");
    printf("Maximum input size: %d bytes\n\n", MAX_INPUT_SIZE);
    
    if (read_input(&input_data) != 0) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (input_data.size == 0) {
        printf("No input data provided\n");
        return 1;
    }
    
    simple_compress(&input_data, &compressed_data);
    
    printf("\nCompression Results:\n");
    display_compression_stats(&input_data, &compressed_data);
    display_compressed_data(&compressed_data);
    
    return 0;
}