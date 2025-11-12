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
        if (!isprint((unsigned char)input[i]) && !isspace((unsigned char)input[i])) return 0;
    }
    return 1;
}

Buffer compress_rle(const char* input, size_t len) {
    Buffer result = {0};
    if (len == 0) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = (uint8_t)input[i];
        size_t count = 1;
        
        while (i + count < len && count < 255 && input[i + count] == current) {
            count++;
        }
        
        if (count > 1 || current == 0xFF) {
            if (out_idx + 3 > MAX_OUTPUT_SIZE) break;
            result.data[out_idx++] = 0xFF;
            result.data[out_idx++] = (uint8_t)count;
            result.data[out_idx++] = current;
        } else {
            if (out_idx + 1 > MAX_OUTPUT_SIZE) break;
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
            
            if (count == 0) {
                i += 3;
                continue;
            }
            
            if (out_idx + count > MAX_OUTPUT_SIZE) break;
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
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (buf->size % 16 != 0) printf("\n");
}

void print_buffer_text(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        if (isprint((unsigned char)buf->data[i])) {
            printf("%c", buf->data[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
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
    
    printf("\nOriginal text (%zu bytes):\n", len);
    printf("%s\n", input);
    
    Buffer compressed = compress_rle(input, len);
    printf("\nCompressed data (%zu bytes):\n", compressed.size);
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    printf("\nDecompressed text (%zu bytes):\n", decompressed.size);
    print_buffer_text(&decompressed);
    
    if (len == decompressed.size && memcmp(input, decompressed.data, len) == 0) {
        printf("\nCompression/Decompression successful!\n");
        if (compressed.size < len) {
            double ratio = (1.0 - (double)compressed.size / len) * 100;
            printf("Compression ratio: %.2f%%\n", ratio);
        } else {
            printf("No compression achieved\n");
        }
    } else {
        printf("\nCompression/Decompression failed!\n");
        return 1;
    }
    
    return 0;
}