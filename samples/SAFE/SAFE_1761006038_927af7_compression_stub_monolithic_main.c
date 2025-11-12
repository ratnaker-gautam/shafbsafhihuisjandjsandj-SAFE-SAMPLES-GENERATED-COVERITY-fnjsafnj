//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

int read_input(Buffer *buf) {
    size_t capacity = 4096;
    buf->data = malloc(capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    
    int c;
    while ((c = getchar()) != EOF) {
        if (buf->size >= MAX_INPUT_SIZE) {
            free(buf->data);
            return 0;
        }
        if (buf->size >= capacity) {
            if (capacity > MAX_INPUT_SIZE / 2) {
                free(buf->data);
                return 0;
            }
            capacity *= 2;
            uint8_t *new_data = realloc(buf->data, capacity);
            if (new_data == NULL) {
                free(buf->data);
                return 0;
            }
            buf->data = new_data;
        }
        buf->data[buf->size++] = (uint8_t)c;
    }
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
    size_t max_output = input->size * 2;
    output->data = malloc(max_output);
    if (output->data == NULL) {
        output->size = 0;
        return;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < 255) {
            count++;
        }
        
        if (out_pos + 2 > max_output) {
            free(output->data);
            output->size = 0;
            return;
        }
        
        output->data[out_pos++] = (uint8_t)count;
        output->data[out_pos++] = current;
        in_pos += count;
    }
    
    output->size = out_pos;
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    if (!read_input(&input)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (input.size == 0) {
        fprintf(stderr, "No input data\n");
        free(input.data);
        return 1;
    }
    
    simple_compress(&input, &output);
    free(input.data);
    
    if (output.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    for (size_t i = 0; i < output.size; i++) {
        if (putchar(output.data[i]) == EOF) {
            fprintf(stderr, "Output error\n");
            free(output.data);
            return 1;
        }
    }
    
    free(output.data);
    return 0;
}