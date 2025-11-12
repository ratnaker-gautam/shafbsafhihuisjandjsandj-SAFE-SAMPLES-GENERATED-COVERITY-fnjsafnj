//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    size_t current = start + 1;
    
    while (current < end && data[current] == data[start] && count < 255) {
        count++;
        current++;
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
        size_t literal_count = current - start;
        result.data[0] = (uint8_t)(literal_count - 1);
        memcpy(result.data + 1, data + start, literal_count);
        result.size = 1 + literal_count;
        
        Buffer rest = compress_chunk(data, start + literal_count, end);
        if (result.size + rest.size < MAX_INPUT_SIZE) {
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

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    
    int c;
    while ((c = getchar()) != EOF && c != '\n' && input_size < MAX_INPUT_SIZE) {
        input[input_size++] = (uint8_t)c;
    }
    
    if (!validate_input(input, input_size)) {
        printf("Invalid input\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_size - compressed.size) / input_size * 100);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%02X ", compressed.data[i]);
    }
    printf("\n");
    
    return 0;
}