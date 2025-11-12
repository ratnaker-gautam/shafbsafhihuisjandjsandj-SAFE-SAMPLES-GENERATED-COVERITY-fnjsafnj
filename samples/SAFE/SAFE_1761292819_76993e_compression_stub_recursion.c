//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void recursive_compress(const uint8_t* input, size_t pos, size_t len, Buffer* output) {
    if (pos >= len) {
        return;
    }
    
    uint8_t current = input[pos];
    size_t count = 1;
    
    size_t next_pos = pos + 1;
    while (next_pos < len && input[next_pos] == current && count < 255) {
        count++;
        next_pos++;
    }
    
    if (output->length + 2 <= MAX_INPUT_SIZE) {
        output->data[output->length++] = current;
        output->data[output->length++] = (uint8_t)count;
    }
    
    recursive_compress(input, next_pos, len, output);
}

int compress_data(const uint8_t* input, size_t input_len, Buffer* output) {
    if (!validate_input((const char*)input, input_len) || output == NULL) {
        return 0;
    }
    
    output->length = 0;
    recursive_compress(input, 0, input_len, output);
    return 1;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X", buf->data[i]);
        if (i < buf->length - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main() {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (!validate_input(input_str, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original: %s\n", input_str);
    printf("Original length: %zu\n", input_len);
    
    if (compress_data((uint8_t*)input_str, input_len, &compressed)) {
        printf("Compressed (hex): ");
        print_buffer_hex(&compressed);
        printf("Compressed length: %zu\n", compressed.length);
        printf("Compression ratio: %.2f%%\n", 
               (float)compressed.length / input_len * 100);
    } else {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    return 0;
}