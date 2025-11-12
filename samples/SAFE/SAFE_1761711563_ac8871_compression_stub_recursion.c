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
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) {
            return 0;
        }
    }
    return 1;
}

void compress_run_length_recursive(const uint8_t* input, size_t len, size_t pos, Buffer* output) {
    if (pos >= len) {
        return;
    }
    
    uint8_t current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (output->length + 2 <= MAX_INPUT_SIZE) {
        output->data[output->length++] = current;
        output->data[output->length++] = (uint8_t)count;
    }
    
    compress_run_length_recursive(input, len, pos + count, output);
}

int compress_data(const char* input, size_t input_len, Buffer* output) {
    if (!validate_input(input, input_len)) {
        return 0;
    }
    
    output->length = 0;
    compress_run_length_recursive((const uint8_t*)input, input_len, 0, output);
    
    return output->length > 0;
}

void print_compressed(const Buffer* buffer) {
    for (size_t i = 0; i < buffer->length; i += 2) {
        if (i + 1 < buffer->length) {
            printf("%c%u", buffer->data[i], (unsigned int)buffer->data[i + 1]);
        }
    }
    printf("\n");
}

int main(void) {
    char input[MAX_INPUT_SIZE + 1];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!compress_data(input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original length: %zu\n", input_len);
    printf("Compressed length: %zu\n", compressed.length);
    printf("Compressed data: ");
    print_compressed(&compressed);
    
    return 0;
}