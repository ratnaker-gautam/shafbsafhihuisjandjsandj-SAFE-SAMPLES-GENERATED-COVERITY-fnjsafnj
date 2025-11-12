//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

Buffer compress_data(const char* input, size_t len) {
    Buffer result = {0};
    if (len == 0) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        if (out_idx >= MAX_OUTPUT_SIZE - 2) break;
        
        char current = input[i];
        size_t count = 1;
        
        while (i + count < len && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == '\\' || current == '~') {
            result.data[out_idx++] = '\\';
            result.data[out_idx++] = (uint8_t)count;
            result.data[out_idx++] = current;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (out_idx >= MAX_OUTPUT_SIZE) break;
                result.data[out_idx++] = current;
            }
        }
        
        i += count;
    }
    
    result.size = out_idx;
    return result;
}

Buffer decompress_data(const uint8_t* input, size_t len) {
    Buffer result = {0};
    if (len == 0) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        if (out_idx >= MAX_OUTPUT_SIZE) break;
        
        if (input[i] == '\\' && i + 2 < len) {
            uint8_t count = input[i + 1];
            char character = (char)input[i + 2];
            
            if (count > 0 && out_idx + count <= MAX_OUTPUT_SIZE) {
                for (uint8_t j = 0; j < count; j++) {
                    result.data[out_idx++] = character;
                }
            }
            i += 3;
        } else {
            result.data[out_idx++] = (char)input[i++];
        }
    }
    
    result.size = out_idx;
    return result;
}

void print_buffer(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", len);
    
    Buffer compressed = compress_data(input, len);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           len > 0 ? (100.0 * compressed.size / len) : 0.0);
    
    Buffer decompressed = decompress_data(compressed.data, compressed.size);
    
    if (decompressed.size == len && 
        memcmp(input, decompressed.data, len) == 0) {
        printf("Decompression verified successfully\n");
    } else {
        printf("Decompression verification failed\n");
        return 1;
    }
    
    return 0;
}