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
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter hex data (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_size = 0;
    char* ptr = line;
    
    while (*ptr != '\0' && *ptr != '\n' && input_size < MAX_INPUT_SIZE) {
        if (ptr[0] == ' ' || ptr[0] == '\t') {
            ptr++;
            continue;
        }
        
        if (ptr[0] == '\0' || ptr[1] == '\0') break;
        
        unsigned int byte;
        if (sscanf(ptr, "%2x", &byte) != 1) {
            fprintf(stderr, "Invalid hex data\n");
            return 1;
        }
        
        input[input_size++] = (uint8_t)byte;
        ptr += 2;
    }
    
    if (!validate_input(input, input_size)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Original data:\n");
    for (size_t i = 0; i < input_size; i++) {
        printf("%02X", input[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (input_size > 0) printf("\n");
    
    Buffer compressed = compress_chunk(input, 0, input_size);
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data:\n");
    print_buffer(&compressed);
    
    if (compressed.size > 0) {
        double ratio = (double)compressed.size / input_size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    return 0;
}