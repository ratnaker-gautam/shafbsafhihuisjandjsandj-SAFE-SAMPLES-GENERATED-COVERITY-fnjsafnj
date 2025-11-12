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
    
    if (left.size + right.size < MAX_INPUT_SIZE) {
        memcpy(result.data, left.data, left.size);
        memcpy(result.data + left.size, right.data, right.size);
        result.size = left.size + right.size;
    }
    
    return result;
}

int validate_input(const uint8_t* data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_INPUT_SIZE) return 0;
    return 1;
}

void print_buffer(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    char line[256];
    size_t input_size = 0;
    
    printf("Enter up to %zu bytes as hex values (e.g., 01 A2 FF): ", MAX_INPUT_SIZE);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte_val;
        if (sscanf(token, "%02X", &byte_val) == 1) {
            if (byte_val <= 0xFF) {
                input[input_size++] = (uint8_t)byte_val;
            }
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (!validate_input(input, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X", input[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (input_size > 0) printf("\n");
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer(&compressed);
    
    double ratio = input_size > 0 ? (double)compressed.size / input_size : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}