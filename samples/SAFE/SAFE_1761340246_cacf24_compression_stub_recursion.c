//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
} Buffer;

Buffer compress_chunk(const unsigned char *input, size_t start, size_t end) {
    Buffer result = {NULL, 0};
    if (start >= end) return result;
    
    size_t compressed_size = 0;
    unsigned char *compressed = malloc(end - start);
    if (!compressed) return result;
    
    size_t i = start;
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < end && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count > 3 || current == 0xFF) {
            compressed[compressed_size++] = 0xFF;
            compressed[compressed_size++] = current;
            compressed[compressed_size++] = (unsigned char)count;
            i = j;
        } else {
            for (size_t k = 0; k < count; k++) {
                compressed[compressed_size++] = current;
            }
            i = j;
        }
    }
    
    result.data = compressed;
    result.size = compressed_size;
    return result;
}

Buffer recursive_compress(const unsigned char *input, size_t start, size_t end, int depth) {
    if (depth <= 0 || (end - start) <= 64) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(input, start, mid, depth - 1);
    Buffer right = recursive_compress(input, mid, end, depth - 1);
    
    Buffer result = {NULL, 0};
    result.size = left.size + right.size;
    result.data = malloc(result.size);
    if (!result.data) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    memcpy(result.data, left.data, left.size);
    memcpy(result.data + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    return result;
}

void print_hex(const unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main() {
    char input_line[MAX_INPUT_SIZE + 1];
    printf("Enter data to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_line, sizeof(input_line), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_len);
    print_hex((unsigned char*)input_line, input_len);
    
    int recursion_depth = 3;
    if (input_len < 100) recursion_depth = 2;
    if (input_len < 50) recursion_depth = 1;
    
    Buffer compressed = recursive_compress((unsigned char*)input_line, 0, input_len, recursion_depth);
    
    if (!compressed.data) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size);
    
    if (compressed.size < input_len) {
        double ratio = (double)compressed.size / input_len * 100;
        printf("Compression ratio: %.1f%%\n", ratio);
    } else {
        printf("No compression achieved\n");
    }
    
    free(compressed.data);
    return 0;
}