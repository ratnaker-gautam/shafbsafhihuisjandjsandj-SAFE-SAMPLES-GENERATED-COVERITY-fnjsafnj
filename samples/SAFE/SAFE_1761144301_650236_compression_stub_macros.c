//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define BYTE_MASK 0xFF
#define HIGH_NIBBLE_SHIFT 4
#define LOW_NIBBLE_MASK 0x0F
#define COMPRESSION_FLAG 0x80
#define RUN_LENGTH_MAX 127

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0) return 0;
    if (len > MAX_INPUT_SIZE) return 0;
    return 1;
}

Buffer compress_rle(const char *input, size_t len) {
    Buffer result = {NULL, 0};
    if (!validate_input(input, len)) return result;
    
    result.data = malloc(len * COMPRESSED_MULTIPLIER);
    if (result.data == NULL) return result;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < len) {
        uint8_t current = (uint8_t)input[i];
        size_t run_length = 1;
        
        while (i + run_length < len && run_length <= RUN_LENGTH_MAX && 
               (uint8_t)input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length > 1) {
            result.data[out_index++] = COMPRESSION_FLAG | (run_length & RUN_LENGTH_MAX);
            result.data[out_index++] = current;
            i += run_length;
        } else {
            result.data[out_index++] = current;
            i++;
        }
        
        if (out_index >= len * COMPRESSED_MULTIPLIER) {
            break;
        }
    }
    
    result.size = out_index;
    return result;
}

Buffer decompress_rle(const uint8_t *input, size_t len) {
    Buffer result = {NULL, 0};
    if (input == NULL || len == 0) return result;
    
    result.data = malloc(MAX_INPUT_SIZE);
    if (result.data == NULL) return result;
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < len && out_index < MAX_INPUT_SIZE) {
        uint8_t current = input[i];
        
        if (current & COMPRESSION_FLAG) {
            size_t run_length = current & RUN_LENGTH_MAX;
            i++;
            
            if (i >= len) break;
            
            uint8_t value = input[i];
            for (size_t j = 0; j < run_length && out_index < MAX_INPUT_SIZE; j++) {
                result.data[out_index++] = value;
            }
            i++;
        } else {
            result.data[out_index++] = current;
            i++;
        }
    }
    
    result.size = out_index;
    return result;
}

void print_buffer_hex(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (len % 16 != 0) printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", len);
    
    Buffer compressed = compress_rle(input, len);
    if (compressed.data == NULL) {
        printf("Compression failed\n");
        return 1;
    }
    
    printf("Compressed (%zu bytes): ", compressed.size);
    print_buffer_hex(compressed.data, compressed.size);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    if (decompressed.data == NULL) {
        printf("Decompression failed\n");
        free(compressed.data);
        return 1;
    }
    
    printf("Decompressed: %s\n", decompressed.data);
    printf("Decompressed size: %zu bytes\n", decompressed.size);
    
    if (len == decompressed.size && memcmp(input, decompressed.data, len) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    double ratio = (len > 0) ? (double)compressed.size / len * 100.0 : 0.0;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    free(compressed.data);
    free(decompressed.data);
    return 0;
}