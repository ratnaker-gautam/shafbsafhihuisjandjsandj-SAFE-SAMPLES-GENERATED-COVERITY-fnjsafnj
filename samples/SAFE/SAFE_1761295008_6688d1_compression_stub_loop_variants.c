//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

void initialize_buffer(Buffer *buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

void simple_compress(const Buffer *input, Buffer *output) {
    if (input->length == 0) return;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < input->length) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input->length && input->data[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) break;
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        i += count;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer *input, Buffer *output) {
    if (input->length == 0 || input->length % 2 != 0) return;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->length; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_idx + count >= MAX_OUTPUT_SIZE) break;
        
        size_t k = 0;
        do {
            output->data[out_idx++] = value;
            k++;
        } while (k < count);
    }
    
    output->length = out_idx;
}

int main(void) {
    Buffer input, compressed, decompressed;
    char user_input[MAX_INPUT_SIZE];
    
    initialize_buffer(&input);
    initialize_buffer(&compressed);
    initialize_buffer(&decompressed);
    
    printf("Enter text to compress: ");
    if (fgets(user_input, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[--input_len] = '\0';
    }
    
    if (!validate_input(user_input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    memcpy(input.data, user_input, input_len);
    input.length = input_len;
    
    simple_compress(&input, &compressed);
    
    printf("Original size: %zu bytes\n", input.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    
    if (compressed.length > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (float)(input.length - compressed.length) * 100.0f / input.length);
    }
    
    simple_decompress(&compressed, &decompressed);
    
    if (decompressed.length == input.length && 
        memcmp(decompressed.data, input.data, input.length) == 0) {
        printf("Decompression successful\n");
    } else {
        printf("Decompression failed\n");
        return 1;
    }
    
    return 0;
}