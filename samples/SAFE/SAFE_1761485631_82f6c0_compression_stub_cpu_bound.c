//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHUNK_SIZE 1024
#define MAX_INPUT_SIZE (1024 * 1024)

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int read_input(Buffer *buf) {
    size_t total_read = 0;
    while (total_read < MAX_INPUT_SIZE) {
        int c = getchar();
        if (c == EOF) {
            break;
        }
        if (total_read < MAX_INPUT_SIZE) {
            buf->data[total_read++] = (uint8_t)c;
        } else {
            return -1;
        }
    }
    buf->size = total_read;
    return 0;
}

void simple_compress(const Buffer *input, Buffer *output) {
    output->size = 0;
    if (input->size == 0) {
        return;
    }
    
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < UCHAR_MAX) {
            count++;
        }
        
        if (output->size + 2 <= MAX_INPUT_SIZE) {
            output->data[output->size++] = (uint8_t)count;
            output->data[output->size++] = current;
        } else {
            break;
        }
        
        i += count;
    }
}

void write_output(const Buffer *buf) {
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
}

int main(void) {
    Buffer input = {0};
    Buffer output = {0};
    
    if (read_input(&input) != 0) {
        fprintf(stderr, "Input too large\n");
        return EXIT_FAILURE;
    }
    
    simple_compress(&input, &output);
    write_output(&output);
    
    return EXIT_SUCCESS;
}