//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0) return 0;
    if (len > MAX_INPUT_SIZE) return 0;
    return 1;
}

Buffer compress_data(const char* input, size_t len) {
    Buffer result = {0};
    if (!validate_input(input, len)) return result;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < len && out_idx < MAX_OUTPUT_SIZE - 2) {
        char current = input[in_idx];
        size_t count = 1;
        
        while (in_idx + count < len && count < UCHAR_MAX && 
               input[in_idx + count] == current) {
            count++;
        }
        
        if (count > 3 || current == '\\') {
            if (out_idx + 3 >= MAX_OUTPUT_SIZE) break;
            result.data[out_idx++] = '\\';
            result.data[out_idx++] = (uint8_t)count;
            result.data[out_idx++] = current;
        } else {
            for (size_t i = 0; i < count && out_idx < MAX_OUTPUT_SIZE; i++) {
                result.data[out_idx++] = current;
            }
        }
        
        in_idx += count;
    }
    
    result.size = out_idx;
    return result;
}

Buffer decompress_data(const uint8_t* input, size_t len) {
    Buffer result = {0};
    if (input == NULL || len == 0) return result;
    
    size_t out_idx = 0;
    size_t in_idx = 0;
    
    while (in_idx < len && out_idx < MAX_OUTPUT_SIZE) {
        if (input[in_idx] == '\\' && in_idx + 2 < len) {
            uint8_t count = input[in_idx + 1];
            char character = (char)input[in_idx + 2];
            
            if (out_idx + count > MAX_OUTPUT_SIZE) break;
            
            for (uint8_t i = 0; i < count; i++) {
                result.data[out_idx++] = character;
            }
            
            in_idx += 3;
        } else {
            result.data[out_idx++] = (char)input[in_idx++];
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

int compare_buffers(const Buffer* a, const Buffer* b) {
    if (a == NULL || b == NULL) return 0;
    if (a->size != b->size) return 0;
    return memcmp(a->data, b->data, a->size) == 0;
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1] = {0};
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE);
    
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
    
    printf("Original: ");
    printf("%s\n", input);
    printf("Original size: %zu bytes\n", len);
    
    Buffer compressed = compress_data(input, len);
    printf("Compressed: ");
    print_buffer(&compressed);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        Buffer decompressed = decompress_data(compressed.data, compressed.size);
        printf("Decompressed: ");
        print_buffer(&decompressed);
        
        if (compare_buffers(&(Buffer){{0}, len}, &decompressed)) {
            printf("Verification: SUCCESS\n");
        } else {
            printf("Verification: FAILED\n");
            return 1;
        }
    }
    
    return 0;
}