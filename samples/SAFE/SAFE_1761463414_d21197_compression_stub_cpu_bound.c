//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (10 * 1024 * 1024)

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buf = {NULL, 0};
    size_t capacity = CHUNK_SIZE;
    buf.data = malloc(capacity);
    if (!buf.data) return buf;
    
    size_t pos = 0;
    int c;
    while ((c = getchar()) != EOF) {
        if (pos >= MAX_INPUT_SIZE) {
            free(buf.data);
            buf.data = NULL;
            return buf;
        }
        
        if (pos >= capacity) {
            if (capacity > MAX_INPUT_SIZE / 2) {
                free(buf.data);
                buf.data = NULL;
                return buf;
            }
            capacity *= 2;
            if (capacity > MAX_INPUT_SIZE) {
                capacity = MAX_INPUT_SIZE;
            }
            uint8_t* new_data = realloc(buf.data, capacity);
            if (!new_data) {
                free(buf.data);
                buf.data = NULL;
                return buf;
            }
            buf.data = new_data;
        }
        
        buf.data[pos++] = (uint8_t)c;
    }
    
    buf.size = pos;
    return buf;
}

void run_length_encode(const Buffer* input, Buffer* output) {
    output->data = NULL;
    output->size = 0;
    
    if (!input || !input->data || input->size == 0) {
        return;
    }
    
    if (input->size > SIZE_MAX / 2) {
        return;
    }
    
    size_t max_output_size = input->size * 2;
    output->data = malloc(max_output_size);
    if (!output->data) {
        return;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && 
               input->data[in_pos + count] == current && 
               count < UINT8_MAX) {
            count++;
        }
        
        if (out_pos + 2 > max_output_size) {
            free(output->data);
            output->data = NULL;
            output->size = 0;
            return;
        }
        
        output->data[out_pos++] = (uint8_t)count;
        output->data[out_pos++] = current;
        in_pos += count;
    }
    
    output->size = out_pos;
}

void write_output(const Buffer* buf) {
    if (!buf || !buf->data) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        if (putchar(buf->data[i]) == EOF) {
            break;
        }
    }
}

int main(void) {
    Buffer input = read_input();
    if (!input.data) {
        return EXIT_FAILURE;
    }
    
    Buffer output;
    run_length_encode(&input, &output);
    free(input.data);
    
    if (!output.data) {
        return EXIT_FAILURE;
    }
    
    write_output(&output);
    free(output.data);
    
    return EXIT_SUCCESS;
}