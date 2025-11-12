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
        result.data[1] = 1;
        result.size = 2;
        return result;
    }
    
    size_t count = 1;
    size_t current = start + 1;
    
    while (current < end && data[current] == data[start] && count < 255) {
        count++;
        current++;
    }
    
    if (count > 1) {
        result.data[0] = data[start];
        result.data[1] = (uint8_t)count;
        result.size = 2;
        
        Buffer rest = compress_chunk(data, current, end);
        if (result.size + rest.size <= MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    } else {
        size_t literal_end = start + 1;
        while (literal_end < end && data[literal_end] != data[literal_end - 1] && 
               (literal_end - start) < 127) {
            literal_end++;
        }
        
        result.data[0] = (uint8_t)(literal_end - start);
        memcpy(result.data + 1, data + start, literal_end - start);
        result.size = 1 + (literal_end - start);
        
        Buffer rest = compress_chunk(data, literal_end, end);
        if (result.size + rest.size <= MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
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
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter data as hex bytes (e.g., '41 42 43'): ");
    
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
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X ", input[i]);
    }
    printf("\n");
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer(&compressed);
    
    double ratio = (input_size > 0) ? (double)compressed.size / input_size : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}