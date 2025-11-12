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
        
        if (out_idx + 2 > MAX_OUTPUT_SIZE) break;
        
        result.data[out_idx++] = (uint8_t)count;
        result.data[out_idx++] = current;
        i += count;
    }
    
    result.size = out_idx;
    return result;
}

Buffer decompress_rle(const uint8_t* data, size_t len) {
    Buffer result = {0};
    if (len == 0 || len % 2 != 0) return result;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len && out_idx < MAX_OUTPUT_SIZE) {
        uint8_t count = data[i++];
        uint8_t value = data[i++];
        
        if (out_idx + count > MAX_OUTPUT_SIZE) break;
        
        for (uint8_t j = 0; j < count; j++) {
            result.data[out_idx++] = value;
        }
    }
    
    result.size = out_idx;
    return result;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 2 == 0) printf(" ");
    }
    if (buf->size > 0) printf("\n");
}

void print_buffer_text(const Buffer* buf) {
    for (size_t i = 0; i < buf->size; i++) {
        if (isprint(buf->data[i]) || isspace(buf->data[i])) {
            putchar(buf->data[i]);
        } else {
            putchar('.');
        }
    }
    printf("\n");
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
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compressed data (hex):\n");
    print_buffer_hex(&compressed);
    
    Buffer decompressed = decompress_rle(compressed.data, compressed.size);
    printf("Decompressed text: ");
    print_buffer_text(&decompressed);
    
    if (decompressed.size == len && memcmp(decompressed.data, input, len) == 0) {
        printf("Compression/Decompression successful\n");
    } else {
        printf("Compression/Decompression failed\n");
        return 1;
    }
    
    return 0;
}