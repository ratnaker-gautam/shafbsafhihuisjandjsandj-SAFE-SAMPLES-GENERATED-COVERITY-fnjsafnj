//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1048576

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

Buffer read_input(void) {
    Buffer buffer = {NULL, 0};
    size_t capacity = 4096;
    buffer.data = malloc(capacity);
    if (buffer.data == NULL) return buffer;
    
    int c;
    while ((c = getchar()) != EOF) {
        if (buffer.size >= capacity) {
            if (capacity >= MAX_INPUT_SIZE) break;
            capacity *= 2;
            if (capacity > MAX_INPUT_SIZE) capacity = MAX_INPUT_SIZE;
            uint8_t* new_data = realloc(buffer.data, capacity);
            if (new_data == NULL) break;
            buffer.data = new_data;
        }
        buffer.data[buffer.size++] = (uint8_t)c;
    }
    
    return buffer;
}

Buffer compress_rle(const Buffer* input) {
    Buffer output = {NULL, 0};
    if (input->size == 0) return output;
    
    output.data = malloc(input->size * 2);
    if (output.data == NULL) return output;
    
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && count < 255 && 
               input->data[i + count] == current) {
            count++;
        }
        
        if (output.size + 2 > input->size * 2) break;
        output.data[output.size++] = (uint8_t)count;
        output.data[output.size++] = current;
        i += count;
    }
    
    return output;
}

void write_output(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        putchar(buffer->data[i]);
    }
}

int main(void) {
    Buffer input = read_input();
    if (input.data == NULL) {
        if (ferror(stdin)) return EXIT_FAILURE;
    }
    
    Buffer compressed = compress_rle(&input);
    if (compressed.data == NULL && input.size > 0) {
        free(input.data);
        return EXIT_FAILURE;
    }
    
    write_output(&compressed);
    
    free(input.data);
    free(compressed.data);
    return EXIT_SUCCESS;
}