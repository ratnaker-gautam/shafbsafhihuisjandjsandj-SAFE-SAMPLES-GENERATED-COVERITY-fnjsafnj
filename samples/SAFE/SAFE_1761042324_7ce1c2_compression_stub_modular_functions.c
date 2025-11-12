//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: compression_stub
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

Buffer compress_rle(const char* input, size_t len) {
    Buffer result = {0};
    if (len == 0) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 1 || current == 0xFF) {
            result.data[out_idx++] = 0xFF;
            result.data[out_idx++] = (uint8_t)count;
            result.data[out_idx++] = current;
        } else {
            result.data[out_idx++] = current;
        }
        
        i += count;
    }
    
    result.size = out_idx;
    return result;
}

Buffer decompress_rle(const uint8_t* input, size_t len) {
    Buffer result = {0};
    if (len == 0) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < MAX_OUTPUT_SIZE) {
        if (input[i] == 0xFF && i + 2 < len) {
            uint8_t count = input[i + 1];
            uint8_t value = input[i + 2];
            
            if (count == 0 || out_idx + count > MAX_OUTPUT_SIZE) {
                result.size = 0;
                return result;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                result.data[out_idx++] = value;
            }
            i += 3;
        } else {
            result.data[out_idx++] = input[i++];
        }
    }
    
    result.size = out_idx;
    return result;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
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
    
    printf("Original text: %s\n", input);
    printf("Original size: %zu bytes\n", len);
    
    Buffer compressed = compress_rle(input, len);
    if (compressed.size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.size == 0 || decompressed.size != len) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (memcmp(input, decompressed.data, len) != 0) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Decompression successful: %s\n", decompressed.data);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed.size / len * 100.0f);
    
    return 0;
}