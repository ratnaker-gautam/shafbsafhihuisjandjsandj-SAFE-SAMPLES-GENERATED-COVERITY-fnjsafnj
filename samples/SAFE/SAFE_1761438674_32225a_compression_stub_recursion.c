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
    
    if (start >= end || input == NULL) {
        return result;
    }
    
    size_t compressed_size = (end - start) * 2;
    unsigned char* compressed = malloc(compressed_size);
    if (compressed == NULL) {
        return result;
    }
    
    size_t write_pos = 0;
    size_t read_pos = start;
    
    while (read_pos < end) {
        unsigned char current = input[read_pos];
        size_t count = 1;
        
        while (read_pos + count < end && count < 255 && 
               input[read_pos + count] == current) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (write_pos + 2 > compressed_size) {
                free(compressed);
                return result;
            }
            compressed[write_pos++] = 0xFF;
            compressed[write_pos++] = (unsigned char)count;
            compressed[write_pos++] = current;
        } else {
            if (write_pos + count > compressed_size) {
                free(compressed);
                return result;
            }
            for (size_t i = 0; i < count; i++) {
                compressed[write_pos++] = current;
            }
        }
        
        read_pos += count;
    }
    
    unsigned char* final = realloc(compressed, write_pos);
    if (final == NULL) {
        free(compressed);
        return result;
    }
    
    result.data = final;
    result.size = write_pos;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, int depth) {
    Buffer result = {NULL, 0};
    
    if (depth > 10 || start >= end || input == NULL) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = recursive_compress(input, start, mid, depth + 1);
    Buffer right = recursive_compress(input, mid, end, depth + 1);
    
    if (left.data == NULL || right.data == NULL) {
        free(left.data);
        free(right.data);
        return compress_chunk(input, start, end);
    }
    
    size_t total_size = left.size + right.size;
    unsigned char* combined = malloc(total_size);
    if (combined == NULL) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    memcpy(combined, left.data, left.size);
    memcpy(combined + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    result.data = combined;
    result.size = total_size;
    return result;
}

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input, 0, input_len, 0);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < compressed.size && i < 50; i++) {
        printf("%02X ", compressed.data[i]);
    }
    if (compressed.size > 50) {
        printf("...");
    }
    printf("\n");
    
    free(compressed.data);
    return 0;
}