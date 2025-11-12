//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (end - start == 1) {
        result.data[0] = data[start];
        result.size = 1;
        return result;
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = compress_chunk(data, start, mid);
    Buffer right = compress_chunk(data, mid, end);
    
    if (left.size + right.size > MAX_INPUT_SIZE) {
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    result.size = left.size + right.size;
    
    return result;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) {
        return 0;
    }
    if (size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void print_buffer(const Buffer* buf) {
    if (buf == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if (i < buf->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 3];
    
    printf("Enter data as hex bytes (e.g., 01 2A FF): ");
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = 0;
    char* token = strtok(line, " \t\n");
    
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) != 1) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return 1;
        }
        if (byte > 0xFF) {
            fprintf(stderr, "Byte value out of range: %s\n", token);
            return 1;
        }
        input[input_size++] = (uint8_t)byte;
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input, input_size)) {
        fprintf(stderr, "Invalid input size\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X", input[i]);
        if (i < input_size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    printf("Processed data (%zu bytes): ", compressed.size);
    print_buffer(&compressed);
    
    if (compressed.size > 0 && compressed.size <= input_size) {
        double ratio = (double)compressed.size / input_size * 100.0;
        printf("Size ratio: %.1f%%\n", ratio);
    }
    
    return 0;
}