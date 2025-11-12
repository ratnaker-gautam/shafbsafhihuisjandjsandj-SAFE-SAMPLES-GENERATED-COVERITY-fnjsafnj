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
    
    size_t out_pos = 0;
    size_t i = start;
    
    while (i < end) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < end && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (out_pos + 2 > compressed_size) {
                free(compressed);
                return result;
            }
            compressed[out_pos++] = 0xFF;
            compressed[out_pos++] = (unsigned char)count;
            compressed[out_pos++] = current;
        } else {
            if (out_pos + count > compressed_size) {
                free(compressed);
                return result;
            }
            for (size_t j = 0; j < count; j++) {
                compressed[out_pos++] = current;
            }
        }
        
        i += count;
    }
    
    unsigned char* final = realloc(compressed, out_pos);
    if (final == NULL) {
        free(compressed);
        return result;
    }
    
    result.data = final;
    result.size = out_pos;
    return result;
}

Buffer recursive_compress(const unsigned char* input, size_t start, size_t end, size_t chunk_size) {
    Buffer result = {NULL, 0};
    
    if (input == NULL || start >= end) {
        return result;
    }
    
    if (end - start <= chunk_size) {
        return compress_chunk(input, start, end);
    }
    
    size_t mid = start + (end - start) / 2;
    
    Buffer left = recursive_compress(input, start, mid, chunk_size);
    Buffer right = recursive_compress(input, mid, end, chunk_size);
    
    if (left.data == NULL && right.data == NULL) {
        return result;
    }
    
    size_t total_size = (left.data ? left.size : 0) + (right.data ? right.size : 0);
    unsigned char* combined = malloc(total_size);
    if (combined == NULL) {
        free(left.data);
        free(right.data);
        return result;
    }
    
    size_t pos = 0;
    if (left.data) {
        memcpy(combined, left.data, left.size);
        pos += left.size;
        free(left.data);
    }
    if (right.data) {
        memcpy(combined + pos, right.data, right.size);
        free(right.data);
    }
    
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

int main() {
    char input[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
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
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Original data:\n");
    print_hex((unsigned char*)input, input_len);
    
    Buffer compressed = recursive_compress((unsigned char*)input, 0, input_len, 64);
    
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / input_len * 100.0f);
    printf("Compressed data:\n");
    print_hex(compressed.data, compressed.size);
    
    free(compressed.data);
    return 0;
}