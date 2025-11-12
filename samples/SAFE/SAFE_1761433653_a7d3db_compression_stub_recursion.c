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
    
    while (current + 1 < end && data[current] == data[current + 1] && count < 255) {
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
        size_t literal_count = 0;
        while (current < end && literal_count < count && result.size + 1 < MAX_INPUT_SIZE) {
            if (current + 1 < end && data[current] == data[current + 1]) {
                break;
            }
            result.data[result.size++] = data[current];
            current++;
            literal_count++;
        }
        
        Buffer rest = compress_chunk(data, current, end);
        if (result.size + rest.size < MAX_INPUT_SIZE) {
            memcpy(result.data + result.size, rest.data, rest.size);
            result.size += rest.size;
        }
    }
    
    return result;
}

int read_input(uint8_t* buffer, size_t max_size) {
    printf("Enter data to compress (max %zu bytes): ", max_size);
    
    size_t total_read = 0;
    int c;
    
    while (total_read < max_size - 1 && (c = getchar()) != EOF && c != '\n') {
        if (c >= 0 && c <= 255) {
            buffer[total_read++] = (uint8_t)c;
        }
    }
    
    buffer[total_read] = '\0';
    return total_read > 0 ? 1 : 0;
}

void print_compressed(const Buffer* compressed) {
    printf("Compressed data (%zu bytes): ", compressed->size);
    
    for (size_t i = 0; i < compressed->size; i++) {
        if (i > 0) printf(" ");
        printf("%02X", compressed->data[i]);
    }
    printf("\n");
    
    printf("Original size: %zu bytes\n", compressed->size);
    if (compressed->size > 0) {
        double ratio = (double)compressed->size / (double)compressed->size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    
    if (!read_input(input, MAX_INPUT_SIZE)) {
        fprintf(stderr, "Error: No input data received\n");
        return 1;
    }
    
    size_t input_size = strlen((char*)input);
    if (input_size == 0) {
        fprintf(stderr, "Error: Input data is empty\n");
        return 1;
    }
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    if (compressed.size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    print_compressed(&compressed);
    
    return 0;
}