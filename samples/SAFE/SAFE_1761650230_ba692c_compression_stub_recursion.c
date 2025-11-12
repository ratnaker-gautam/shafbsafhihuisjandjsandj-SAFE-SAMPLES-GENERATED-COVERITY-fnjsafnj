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

Buffer compress_recursive(const uint8_t* input, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (end - start == 1) {
        result.data[0] = input[start];
        result.data[1] = 1;
        result.size = 2;
        return result;
    }
    
    size_t run_length = 1;
    size_t current = start + 1;
    
    while (current < end && input[current] == input[start] && run_length < 255) {
        run_length++;
        current++;
    }
    
    if (run_length > 1) {
        Buffer compressed = compress_recursive(input, current, end);
        if (result.size + 2 + compressed.size <= MAX_INPUT_SIZE) {
            result.data[0] = input[start];
            result.data[1] = (uint8_t)run_length;
            result.size = 2;
            memcpy(result.data + 2, compressed.data, compressed.size);
            result.size += compressed.size;
        }
    } else {
        size_t literal_end = start + 1;
        while (literal_end < end && input[literal_end] != input[literal_end - 1] && 
               (literal_end - start) < 127) {
            literal_end++;
        }
        
        Buffer compressed = compress_recursive(input, literal_end, end);
        size_t literal_length = literal_end - start;
        
        if (result.size + 1 + literal_length + compressed.size <= MAX_INPUT_SIZE) {
            result.data[0] = (uint8_t)(0x80 | (literal_length - 1));
            result.size = 1;
            memcpy(result.data + 1, input + start, literal_length);
            result.size += literal_length;
            memcpy(result.data + result.size, compressed.data, compressed.size);
            result.size += compressed.size;
        }
    }
    
    return result;
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2];
    
    printf("Enter text to compress: ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(line);
    if (input_len > 0 && line[input_len - 1] == '\n') {
        line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_SIZE) {
        printf("Input too long.\n");
        return 1;
    }
    
    memcpy(input, line, input_len);
    
    Buffer compressed = compress_recursive(input, 0, input_len);
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}