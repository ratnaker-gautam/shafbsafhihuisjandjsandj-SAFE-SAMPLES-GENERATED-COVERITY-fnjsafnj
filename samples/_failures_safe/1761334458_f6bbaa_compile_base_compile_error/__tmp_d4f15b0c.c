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

Buffer compress_recursive(const unsigned char* input, size_t len, size_t depth) {
    Buffer result = {NULL, 0};
    
    if (depth > 100 || len == 0) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    if (len == 1) {
        result.data = malloc(2);
        if (result.data) {
            result.data[0] = input[0];
            result.data[1] = 1;
            result.size = 2;
        }
        return result;
    }
    
    size_t compressed_size = 0;
    unsigned char current = input[0];
    size_t count = 1;
    
    for (size_t i = 1; i < len; i++) {
        if (input[i] == current && count < 255) {
            count++;
        } else {
            compressed_size += 2;
            current = input[i];
            count = 1;
        }
    }
    compressed_size += 2;
    
    if (compressed_size >= len) {
        result.data = malloc(len + 1);
        if (result.data) {
            memcpy(result.data, input, len);
            result.data[len] = 0;
            result.size = len + 1;
        }
        return result;
    }
    
    result.data = malloc(compressed_size);
    if (!result.data) {
        result.size = 0;
        return result;
    }
    
    size_t idx = 0;
    current = input[0];
    count = 1;
    
    for (size_t i = 1; i < len; i++) {
        if (input[i] == current && count < 255) {
            count++;
        } else {
            result.data[idx++] = current;
            result.data[idx++] = (unsigned char)count;
            current = input[i];
            count = 1;
        }
    }
    result.data[idx++] = current;
    result.data[idx++] = (unsigned char)count;
    
    result.size = compressed_size;
    
    if (result.size > 8) {
        Buffer recursive_result = compress_recursive(result.data, result.size, depth + 1);
        if (recursive_result.data && recursive_result.size < result.size) {
            free(result.data);
            return recursive_result;
        }
        free(recursive_result.data);
    }
    
    return result;
}

Buffer decompress_recursive(const unsigned char* input, size_t len, size_t depth) {
    Buffer result = {NULL, 0};
    
    if (depth > 100 || len == 0) {
        result.data = malloc(1);
        if (result.data) {
            result.data[0] = 0;
            result.size = 1;
        }
        return result;
    }
    
    if (len % 2 != 0) {
        size_t output_size = 0;
        for (size_t i = 0; i < len; i++) {
            if (i + 1 < len && input[i + 1] > 0) {
                output_size += input[i + 1];
                i++;
            } else {
                output_size++;
            }
        }
        
        result.data = malloc(output_size);
        if (!result.data) {
            result.size = 0;
            return result;
        }
        
        size_t idx = 0;
        for (size_t i = 0; i < len; i++) {
            if (i + 1 < len && input[i + 1] > 0) {
                unsigned char value = input[i];
                unsigned char count = input[i + 1];
                for (unsigned char j = 0; j < count; j++) {
                    if (idx < output_size) {
                        result.data[idx++] = value;
                    }
                }
                i++;
            } else {
                if (idx < output_size) {
                    result.data[idx++] = input[i];
                }
            }
        }
        result.size = idx;
        
        if (result.size > 8) {
            Buffer recursive_result = decompress_recursive(result.data, result.size, depth + 1);
            if (recursive_result.data) {
                free(result.data);
                return recursive_result;
            }
        }
    } else {
        result.data = malloc(len);
        if (result.data) {
            memcpy(result.data, input, len);
            result.size = len;
        }
    }
    
    return result;
}

int main() {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    Buffer compressed = compress_recursive((unsigned char*)input, len, 0);
    if (!compressed.data) {
        fprintf(stderr, "Compression failed\n");
        return 1;