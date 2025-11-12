//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

Buffer compress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    if (end - start == 1) {
        result.data[0] = data[start];
        result.length = 1;
        return result;
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = compress_chunk(data, start, mid);
    Buffer right = compress_chunk(data, mid, end);
    
    if (left.length + right.length < MAX_INPUT_SIZE) {
        memcpy(result.data, left.data, left.length);
        memcpy(result.data + left.length, right.data, right.length);
        result.length = left.length + right.length;
    }
    
    return result;
}

int validate_input(const uint8_t* data, size_t length) {
    if (data == NULL) return 0;
    if (length == 0) return 0;
    if (length > MAX_INPUT_SIZE) return 0;
    return 1;
}

void print_buffer(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (buf->length > 0) printf("\n");
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    char line[MAX_INPUT_SIZE * 2 + 1];
    
    printf("Enter hex data (max %d bytes): ", MAX_INPUT_SIZE);
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = 0;
    size_t line_len = strlen(line);
    
    for (size_t i = 0; i < line_len && input_len < MAX_INPUT_SIZE; i += 2) {
        if (line[i] == '\n' || line[i] == '\0') break;
        
        unsigned int byte_val;
        if (sscanf(line + i, "%2x", &byte_val) != 1) {
            fprintf(stderr, "Invalid hex input\n");
            return 1;
        }
        input[input_len++] = (uint8_t)byte_val;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_len);
    for (size_t i = 0; i < input_len; i++) {
        printf("%02X", input[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf(" ");
    }
    if (input_len > 0) printf("\n");
    
    Buffer compressed = compress_chunk(input, 0, input_len);
    
    printf("Compressed data (%zu bytes):\n", compressed.length);
    print_buffer(&compressed);
    
    if (compressed.length > 0) {
        double ratio = (double)compressed.length / (double)input_len * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    return 0;
}