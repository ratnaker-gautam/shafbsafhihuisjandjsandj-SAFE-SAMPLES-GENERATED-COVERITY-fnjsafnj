//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

void recursive_compress(const unsigned char* input, size_t pos, size_t len, Buffer* output) {
    if (pos >= len) {
        return;
    }
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    size_t next_pos = pos + 1;
    while (next_pos < len && input[next_pos] == current && count < 255) {
        count++;
        next_pos++;
    }
    
    if (output->length + 2 <= MAX_INPUT_SIZE) {
        output->data[output->length++] = current;
        output->data[output->length++] = (unsigned char)count;
    }
    
    recursive_compress(input, next_pos, len, output);
}

int compress_data(const unsigned char* input, size_t input_len, Buffer* output) {
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
    char user_input[MAX_INPUT_SIZE + 1];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(user_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!compress_data((const unsigned char*)user_input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original length: %zu\n", input_len);
    printf("Compressed length: %zu\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           (input_len > 0) ? (100.0 * compressed.length / input_len) : 0.0);
    printf("Compressed data (hex): ");
    print_buffer_hex(&compressed);
    
    return 0;
}