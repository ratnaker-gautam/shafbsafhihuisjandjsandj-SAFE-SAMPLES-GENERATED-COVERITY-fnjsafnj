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

void initialize_buffer(Buffer* buf) {
    memset(buf->data, 0, MAX_INPUT_SIZE);
    buf->length = 0;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_SIZE) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

void simple_compress(const Buffer* input, Buffer* output) {
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
        
        output->data[out_idx++] = current;
        output->data[out_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    output->length = out_idx;
}

void simple_decompress(const Buffer* input, Buffer* output) {
    if (input->length == 0 || input->length % 2 != 0) return;
    
    size_t out_idx = 0;
    
    for (size_t i = 0; i < input->length; i += 2) {
        uint8_t character = input->data[i];
        uint8_t count = input->data[i + 1];
        
        if (out_idx + count >= MAX_OUTPUT_SIZE) break;
        
        size_t k = 0;
        do {
            output->data[out_idx++] = character;
            k++;
        } while (k < count);
    }
    
    output->length = out_idx;
}

void print_buffer_hex(const Buffer* buf) {
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    char user_input[MAX_INPUT_SIZE];
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    printf("Enter text to compress: ");
    if (fgets(user_input, MAX_INPUT_SIZE, stdin) == NULL) {
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
    
    memcpy(input_buf.data, user_input, input_len);
    input_buf.length = input_len;
    
    simple_compress(&input_buf, &compressed_buf);
    simple_decompress(&compressed_buf, &decompressed_buf);
    
    printf("Original: %s\n", user_input);
    printf("Compressed (hex): ");
    print_buffer_hex(&compressed_buf);
    printf("Decompressed: ");
    
    for (size_t i = 0; i < decompressed_buf.length; i++) {
        putchar(decompressed_buf.data[i]);
    }
    printf("\n");
    
    printf("Original size: %zu bytes\n", input_buf.length);
    printf("Compressed size: %zu bytes\n", compressed_buf.length);
    printf("Compression ratio: %.2f%%\n", 
           (float)compressed_buf.length / input_buf.length * 100.0f);
    
    if (memcmp(input_buf.data, decompressed_buf.data, input_buf.length) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
    }
    
    return 0;
}