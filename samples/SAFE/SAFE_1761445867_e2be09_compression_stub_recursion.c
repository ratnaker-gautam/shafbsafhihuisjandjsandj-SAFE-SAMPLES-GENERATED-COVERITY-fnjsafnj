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
    
    while (pos + count < len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (count > 3 || current == '\\' || current == '\'' || current == '\"') {
        if (output->length + 3 < MAX_INPUT_SIZE) {
            output->data[output->length++] = '\\';
            output->data[output->length++] = (unsigned char)count;
            output->data[output->length++] = current;
        }
    } else {
        for (size_t i = 0; i < count; i++) {
            if (output->length < MAX_INPUT_SIZE) {
                output->data[output->length++] = current;
            }
        }
    }
    
    recursive_compress(input, pos + count, len, output);
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
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (buf->length > 0) {
        printf("\n");
    }
}

int main(void) {
    char user_input[MAX_INPUT_SIZE + 1];
    Buffer compressed;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!compress_data((unsigned char*)user_input, input_len, &compressed)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", input_len);
    printf("Compressed size: %zu bytes\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", 
           (float)(input_len - compressed.length) / input_len * 100);
    
    printf("Compressed data (hex):\n");
    print_buffer_hex(&compressed);
    
    return 0;
}