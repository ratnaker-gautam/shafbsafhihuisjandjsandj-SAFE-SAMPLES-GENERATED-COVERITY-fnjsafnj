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

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length > MAX_INPUT_SIZE) return 0;
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_size, uint8_t* output, size_t output_size) {
    if (input == NULL || output == NULL) return 0;
    if (input_size == 0 || output_size < input_size * 2) return 0;
    
    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (output_idx + 2 > output_size) return 0;
        
        output[output_idx++] = (uint8_t)count;
        output[output_idx++] = current;
        i += count;
    }
    
    return output_idx;
}

int initialize_buffer(Buffer* buf) {
    if (buf == NULL) return 0;
    memset(buf->data, 0, sizeof(buf->data));
    buf->size = 0;
    return 1;
}

int process_compression(const char* input_str, Buffer* output_buf) {
    if (!validate_input(input_str, strlen(input_str))) return 0;
    if (output_buf == NULL) return 0;
    
    size_t input_len = strlen(input_str);
    size_t compressed_size = simple_compress((const uint8_t*)input_str, input_len, 
                                           output_buf->data, sizeof(output_buf->data));
    
    if (compressed_size == 0) return 0;
    
    output_buf->size = compressed_size;
    return 1;
}

void display_results(const char* input, const Buffer* output) {
    if (input == NULL || output == NULL) return;
    
    printf("Original: %s\n", input);
    printf("Original size: %zu bytes\n", strlen(input));
    printf("Compressed size: %zu bytes\n", output->size);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output->size; i++) {
        printf("%02X ", output->data[i]);
    }
    printf("\n");
    
    double ratio = (output->size * 100.0) / strlen(input);
    printf("Compression ratio: %.2f%%\n", ratio);
}

int get_user_input(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return 0;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) return 0;
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_SIZE];
    Buffer output_buffer;
    
    if (!initialize_buffer(&output_buffer)) {
        fprintf(stderr, "Failed to initialize buffer\n");
        return 1;
    }
    
    if (!get_user_input(input_buffer, sizeof(input_buffer))) {
        fprintf(stderr, "Invalid input received\n");
        return 1;
    }
    
    if (!process_compression(input_buffer, &output_buffer)) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    display_results(input_buffer, &output_buffer);
    
    return 0;
}