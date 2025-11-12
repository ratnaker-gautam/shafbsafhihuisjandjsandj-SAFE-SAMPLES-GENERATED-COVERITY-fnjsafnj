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
    if (start >= end) return result;
    
    size_t compressed_size = 0;
    unsigned char* compressed = malloc(end - start);
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

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, size_t chunk_size) {
    if (end - start <= chunk_size) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    Buffer left = recursive_compress(input, start, mid, chunk_size);
    Buffer right = recursive_compress(input, mid, end, chunk_size);
    
    if (!left.data || !right.data) {
        free(left.data);
        free(right.data);
        return (Buffer){NULL, 0};
    }
    
    unsigned char* combined = malloc(left.size + right.size);
    if (!combined) {
        free(left.data);
        free(right.data);
        return (Buffer){NULL, 0};
    }
    
    memcpy(combined, left.data, left.size);
    memcpy(combined + left.size, right.data, right.size);
    
    free(left.data);
    free(right.data);
    
    Buffer result;
    result.data = combined;
    result.size = left.size + right.size;
    return result;
}

void print_hex(const unsigned char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main() {
    char input_line[MAX_INPUT_SIZE + 2];
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (!fgets(input_line, sizeof(input_line), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    Buffer compressed = recursive_compress((unsigned char*)input_line, 0, input_len, 32);
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nOriginal size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_len) * 100.0);
    
    printf("\nCompressed data (hex):\n");
    print_hex(compressed.data, compressed.size);
    
    free(compressed.data);
    return 0;
}