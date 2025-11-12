//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    
    size_t compressed_size = 0;
    unsigned char* compressed = malloc(end - start);
    if (compressed == NULL) {
        return result;
    }
    
    size_t i = start;
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < end && input[i + count] == current && count < UCHAR_MAX) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            compressed[compressed_size++] = 0xFF;
            compressed[compressed_size++] = current;
            compressed[compressed_size++] = (unsigned char)count;
        } else {
            for (size_t j = 0; j < count; j++) {
                compressed[compressed_size++] = current;
            }
        }
        
        i += count;
    }
    
    unsigned char* final = realloc(compressed, compressed_size);
    if (final != NULL) {
        compressed = final;
    }
    
    result.data = compressed;
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, int depth) {
    Buffer result = {NULL, 0};
    
    if (depth <= 0 || start >= end || input == NULL) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = recursive_compress(input, start, mid, depth - 1);
    Buffer right = recursive_compress(input, mid, end, depth - 1);
    
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
        return compress_chunk(input, start, end);
    }
    
    memcpy(combined, left.data, left.size);
    memcpy(combined + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    result.data = combined;
    result.size = total_size;
    return result;
}

void print_hex(const unsigned char* data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_len);
    print_hex((unsigned char*)input_buffer, input_len);
    
    int recursion_depth = 3;
    if (input_len < 100) {
        recursion_depth = 2;
    } else if (input_len < 20) {
        recursion_depth = 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input_buffer, 0, input_len, recursion_depth);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    double ratio = (double)compressed.size / input_len * 100;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    free(compressed.data);
    return 0;
}