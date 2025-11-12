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
    
    size_t total_size = left.size + right.size;
    if (total_size > MAX_INPUT_SIZE) {
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    result.size = total_size;
    
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
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter hex string (max %zu bytes): ", MAX_INPUT_SIZE);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len % 2 != 0 || input_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex string length\n");
        return 1;
    }
    
    size_t data_size = input_len / 2;
    for (size_t i = 0; i < data_size; i++) {
        char hex_byte[3] = {line[i * 2], line[i * 2 + 1], '\0'};
        char* endptr;
        unsigned long value = strtoul(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value > 0xFF) {
            fprintf(stderr, "Invalid hex character\n");
            return 1;
        }
        input[i] = (uint8_t)value;
    }
    
    if (!validate_input(input, data_size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", data_size);
    printf("Original data: ");
    for (size_t i = 0; i < data_size; i++) {
        printf("%02X", input[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_chunk(input, 0, data_size);
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data: ");
    print_buffer(&compressed);
    
    return 0;
}