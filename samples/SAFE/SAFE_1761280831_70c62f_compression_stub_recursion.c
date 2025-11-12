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
    
    size_t count = 1;
    size_t current = start;
    
    while (current + 1 < end && data[current] == data[current + 1]) {
        count++;
        current++;
        
        if (count == 255) {
            break;
        }
    }
    
    if (count > 3) {
        result.data[0] = 0xFF;
        result.data[1] = data[start];
        result.data[2] = (uint8_t)count;
        result.size = 3;
        
        Buffer rest = compress_chunk(data, start + count, end);
        if (result.size + rest.size < MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    } else {
        size_t literal_count = count;
        while (current + 1 < end && data[current] != data[current + 1] && literal_count < 127) {
            literal_count++;
            current++;
        }
        
        result.data[0] = (uint8_t)literal_count;
        for (size_t i = 0; i < literal_count; i++) {
            result.data[i + 1] = data[start + i];
        }
        result.size = literal_count + 1;
        
        Buffer rest = compress_chunk(data, start + literal_count, end);
        if (result.size + rest.size < MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    }
    
    return result;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2];
    
    printf("Enter data to compress (hex digits, max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    if (input_len % 2 != 0) {
        fprintf(stderr, "Input must have even number of hex digits\n");
        return 1;
    }
    
    size_t byte_count = input_len / 2;
    if (byte_count > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    for (size_t i = 0; i < byte_count; i++) {
        char hex[3] = {line[i * 2], line[i * 2 + 1], '\0'};
        char* endptr;
        long value = strtol(hex, &endptr, 16);
        
        if (endptr != hex + 2 || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex digit at position %zu\n", i * 2);
            return 1;
        }
        
        input[i] = (uint8_t)value;
    }
    
    Buffer compressed = compress_chunk(input, 0, byte_count);
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X", compressed.data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    if (compressed.size % 16 != 0) {
        printf("\n");
    }
    
    double ratio = byte_count > 0 ? (double)compressed.size / byte_count : 0.0;
    printf("Compression ratio: %.2f\n", ratio);
    
    return 0;
}