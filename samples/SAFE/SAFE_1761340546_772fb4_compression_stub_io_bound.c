//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int read_input(Buffer *buf) {
    if (!buf) return 0;
    
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF || c == '\n') break;
        buf->data[total_read++] = (uint8_t)c;
    }
    buf->size = total_read;
    return 1;
}

int simple_compress(const Buffer *input, Buffer *output) {
    if (!input || !output || input->size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE - 4) {
        uint8_t current = input->data[in_idx];
        size_t count = 1;
        
        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }
        
        if (count > 3 || current == '\\' || current == '\'') {
            output->data[out_idx++] = '\\';
            output->data[out_idx++] = (uint8_t)count;
            output->data[out_idx++] = current;
        } else {
            for (size_t i = 0; i < count && out_idx < MAX_OUTPUT_SIZE; i++) {
                output->data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    output->size = out_idx;
    return 1;
}

int simple_decompress(const Buffer *input, Buffer *output) {
    if (!input || !output || input->size == 0) return 0;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < input->size && out_idx < MAX_OUTPUT_SIZE) {
        if (input->data[in_idx] == '\\' && in_idx + 2 < input->size) {
            uint8_t count = input->data[in_idx + 1];
            uint8_t value = input->data[in_idx + 2];
            
            if (count > 0 && out_idx + count <= MAX_OUTPUT_SIZE) {
                for (uint8_t i = 0; i < count; i++) {
                    output->data[out_idx++] = value;
                }
                in_idx += 3;
            } else {
                output->data[out_idx++] = input->data[in_idx++];
            }
        } else {
            output->data[out_idx++] = input->data[in_idx++];
        }
    }
    
    output->size = out_idx;
    return 1;
}

void write_output(const Buffer *buf) {
    if (!buf) return;
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
    putchar('\n');
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress: ");
    fflush(stdout);
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    printf("Original size: %zu\n", input.size);
    
    if (!simple_compress(&input, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input.size - compressed.size) * 100.0f / input.size);
    
    printf("Compressed: ");
    write_output(&compressed);
    
    if (!simple_decompress(&compressed, &decompressed)) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (decompressed.size != input.size || 
        memcmp(decompressed.data, input.data, input.size) != 0) {
        fprintf(stderr, "Decompression verification failed\n");
        return 1;
    }
    
    printf("Decompression verified successfully\n");
    
    return 0;
}