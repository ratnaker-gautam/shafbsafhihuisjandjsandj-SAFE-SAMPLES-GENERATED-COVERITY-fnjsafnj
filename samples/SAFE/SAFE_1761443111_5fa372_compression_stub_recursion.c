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

Buffer compress_rle(const uint8_t* input, size_t size, size_t pos) {
    Buffer result = {0};
    if (pos >= size) {
        return result;
    }
    
    size_t count = 1;
    while (pos + count < size && input[pos] == input[pos + count] && count < 255) {
        count++;
    }
    
    if (count > 1) {
        Buffer rest = compress_rle(input, size, pos + count);
        result.data[0] = input[pos];
        result.data[1] = (uint8_t)count;
        result.size = 2 + rest.size;
        if (result.size <= MAX_INPUT_SIZE) {
            memcpy(result.data + 2, rest.data, rest.size);
        } else {
            result.size = 0;
        }
    } else {
        size_t literal_count = 1;
        while (pos + literal_count < size && input[pos + literal_count] != input[pos + literal_count - 1] && literal_count < 127) {
            literal_count++;
        }
        
        Buffer rest = compress_rle(input, size, pos + literal_count);
        result.data[0] = (uint8_t)(literal_count | 0x80);
        result.size = 1 + literal_count + rest.size;
        if (result.size <= MAX_INPUT_SIZE) {
            memcpy(result.data + 1, input + pos, literal_count);
            memcpy(result.data + 1 + literal_count, rest.data, rest.size);
        } else {
            result.size = 0;
        }
    }
    
    return result;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 3];
    
    printf("Enter data as hex bytes (e.g., AA BB CC): ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }
    
    size_t input_size = 0;
    char* token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned int byte;
        if (sscanf(token, "%02X", &byte) == 1 && byte <= 0xFF) {
            input[input_size++] = (uint8_t)byte;
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        printf("No valid input data\n");
        return 1;
    }
    
    Buffer compressed = compress_rle(input, input_size, 0);
    if (compressed.size == 0 || compressed.size > MAX_INPUT_SIZE) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.size / input_size) * 100);
    printf("Compressed data: ");
    print_buffer_hex(&compressed);
    
    return 0;
}