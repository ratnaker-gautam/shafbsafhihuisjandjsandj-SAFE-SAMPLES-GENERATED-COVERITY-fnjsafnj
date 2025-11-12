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
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        
        i += count;
    }
    
    output->length = out_idx;
}

void process_input() {
    char user_input[MAX_INPUT_SIZE];
    Buffer input_buf, output_buf;
    
    initialize_buffer(&input_buf);
    initialize_buffer(&output_buf);
    
    printf("Enter text to compress (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(user_input, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(user_input, input_len)) {
        printf("Invalid input\n");
        return;
    }
    
    memcpy(input_buf.data, user_input, input_len);
    input_buf.length = input_len;
    
    simple_compress(&input_buf, &output_buf);
    
    printf("Original size: %zu bytes\n", input_buf.length);
    printf("Compressed size: %zu bytes\n", output_buf.length);
    printf("Compression ratio: %.2f%%\n", 
           (input_buf.length > 0) ? 
           (100.0 * output_buf.length / input_buf.length) : 0.0);
    
    printf("Compressed data (hex): ");
    for (size_t k = 0; k < output_buf.length; k++) {
        printf("%02X ", output_buf.data[k]);
    }
    printf("\n");
}

int main() {
    process_input();
    return 0;
}