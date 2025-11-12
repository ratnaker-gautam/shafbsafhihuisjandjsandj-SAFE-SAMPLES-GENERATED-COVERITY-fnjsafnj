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

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_len >= output_max - 3) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count > 3) {
            output[output_len++] = current;
            output[output_len++] = current;
            output[output_len++] = (uint8_t)count;
        } else {
            for (size_t k = 0; k < count; k++) {
                if (output_len >= output_max) {
                    return 0;
                }
                output[output_len++] = current;
            }
        }
        
        i = j;
    }
    
    return output_len;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (input == NULL || output == NULL || input_len == 0 || output_max == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_len >= output_max) {
            return 0;
        }
        
        if (i + 2 < input_len && input[i] == input[i + 1]) {
            uint8_t value = input[i];
            uint8_t count = input[i + 2];
            
            if (count < 4 || output_len + count > output_max) {
                return 0;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                output[output_len++] = value;
            }
            i += 3;
        } else {
            output[output_len++] = input[i];
            i++;
        }
    }
    
    return output_len;
}

void print_buffer_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    char user_input[MAX_INPUT_SIZE];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
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
    
    memcpy(input_buf.data, user_input, input_len);
    input_buf.length = input_len;
    
    printf("\nOriginal data (%zu bytes):\n", input_buf.length);
    print_buffer_hex(input_buf.data, input_buf.length);
    
    compressed_buf.length = simple_compress(input_buf.data, input_buf.length, 
                                          compressed_buf.data, MAX_OUTPUT_SIZE);
    
    if (compressed_buf.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    printf("\nCompressed data (%zu bytes):\n", compressed_buf.length);
    print_buffer_hex(compressed_buf.data, compressed_buf.length);
    
    decompressed_buf.length = simple_decompress(compressed_buf.data, compressed_buf.length,
                                               decompressed_buf.data, MAX_INPUT_SIZE);
    
    if (decompressed_buf.length == 0) {
        fprintf(stderr, "De