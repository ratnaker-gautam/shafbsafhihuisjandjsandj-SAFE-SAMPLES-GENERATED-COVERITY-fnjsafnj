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
    uint8_t* temp = malloc(MAX_INPUT_SIZE);
    if (!temp) return buf;
    
    size_t total = 0;
    while (total < MAX_INPUT_SIZE) {
        size_t to_read = MAX_INPUT_SIZE - total;
        if (to_read > CHUNK_SIZE) to_read = CHUNK_SIZE;
        
        size_t n = fread(temp + total, 1, to_read, stdin);
        if (n == 0) break;
        total += n;
    }
    
    if (total > 0) {
        buf.data = temp;
        buf.size = total;
    } else {
        free(temp);
    }
    return buf;
}

void simple_compress(const Buffer* input, Buffer* output) {
    if (input->size == 0) return;
    
    size_t max_output = input->size * 2;
    if (max_output < input->size) return;
    
    uint8_t* out_data = malloc(max_output);
    if (!out_data) return;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < UCHAR_MAX) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_pos + 3 > max_output) break;
            out_data[out_pos++] = 0xFF;
            out_data[out_pos++] = current;
            out_data[out_pos++] = (uint8_t)count;
        } else {
            if (out_pos + count > max_output) break;
            for (size_t i = 0; i < count; i++) {
                out_data[out_pos++] = current;
            }
        }
        
        in_pos += count;
    }
    
    output->data = out_data;
    output->size = out_pos;
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    Buffer output = {NULL, 0};
    simple_compress(&input, &output);
    
    if (output.data) {
        size_t written = fwrite(output.data, 1, output.size, stdout);
        if (written != output.size) {
            fprintf(stderr, "Error writing output\n");
        }
        free(output.data);
    }
    
    free(input.data);
    return 0;
}