//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char* data;
    size_t size;
} Buffer;

Buffer compress_chunk(const unsigned char* input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    
    if (start >= end) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    size_t chunk_size = (end - start) / 2;
    if (chunk_size == 0) chunk_size = 1;
    
    Buffer left = compress_chunk(input, start, start + chunk_size);
    Buffer right = compress_chunk(input, start + chunk_size, end);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    result.size = left.size + right.size;
    result.data = malloc(result.size);
    
    if (result.data) {
        memcpy(result.data, left.data, left.size);
        memcpy(result.data + left.size, right.data, right.size);
    }
    
    free(left.data);
    free(right.data);
    return result;
}

int validate_input(const unsigned char* data, size_t size) {
    if (!data || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void process_compression(const unsigned char* input, size_t input_size) {
    if (!validate_input(input, input_size)) {
        printf("Invalid input\n");
        return;
    }
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    if (!compressed.data) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", compressed.size);
    printf("Compression ratio: %.2f\n", (float)compressed.size / input_size);
    
    printf("First 16 bytes: ");
    for (size_t i = 0; i < compressed.size && i < 16; i++) {
        printf("%02x ", compressed.data[i]);
    }
    printf("\n");
    
    free(compressed.data);
}

int main() {
    unsigned char test_data[] = "This is a test string for compression demonstration. It contains various characters and patterns.";
    size_t data_size = sizeof(test_data) - 1;
    
    printf("Starting compression test...\n");
    process_compression(test_data, data_size);
    
    unsigned char user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)user_input, MAX_INPUT_SIZE, stdin)) {
        size_t user_size = strlen((char*)user_input);
        if (user_size > 0 && user_input[user_size - 1] == '\n') {
            user_input[--user_size] = '\0';
        }
        
        if (user_size > 0) {
            process_compression(user_input, user_size);
        } else {
            printf("No input provided\n");
        }
    } else {
        printf("Failed to read input\n");
    }
    
    return 0;
}