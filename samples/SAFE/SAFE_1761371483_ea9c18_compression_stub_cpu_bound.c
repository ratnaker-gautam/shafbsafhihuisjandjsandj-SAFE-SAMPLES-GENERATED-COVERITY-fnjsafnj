//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buf = {NULL, 0};
    size_t capacity = CHUNK_SIZE;
    buf.data = malloc(capacity);
    if (!buf.data) return buf;
    
    int c;
    while ((c = getchar()) != EOF) {
        if (buf.size >= MAX_INPUT_SIZE) break;
        if (buf.size >= capacity) {
            if (capacity > MAX_INPUT_SIZE / 2) break;
            capacity *= 2;
            uint8_t* new_data = realloc(buf.data, capacity);
            if (!new_data) break;
            buf.data = new_data;
        }
        buf.data[buf.size++] = (uint8_t)c;
    }
    return buf;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input->size == 0) return;
    
    size_t out_capacity = input->size * 2;
    output->data = malloc(out_capacity);
    if (!output->data) return;
    
    size_t out_pos = 0;
    size_t i = 0;
    
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && 
               input->data[i + count] == current && 
               count < UINT8_MAX) {
            count++;
        }
        
        if (out_pos + 2 > out_capacity) {
            out_capacity *= 2;
            uint8_t* new_data = realloc(output->data, out_capacity);
            if (!new_data) {
                free(output->data);
                output->data = NULL;
                return;
            }
            output->data = new_data;
        }
        
        output->data[out_pos++] = (uint8_t)count;
        output->data[out_pos++] = current;
        i += count;
    }
    
    output->size = out_pos;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    Buffer compressed = {NULL, 0};
    simple_compress(&input, &compressed);
    
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        free(input.data);
        return 1;
    }
    
    for (size_t i = 0; i < compressed.size; i++) {
        if (putchar(compressed.data[i]) == EOF) {
            fprintf(stderr, "Output error\n");
            break;
        }
    }
    
    free(input.data);
    free(compressed.data);
    return 0;
}