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

int validate_input(const uint8_t* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_len + 3 > output_max) {
            return 0;
        }
        
        uint8_t current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < input_len && input[j] == current && count < 255) {
            count++;
            j++;
        }
        
        if (count >= 3) {
            output[output_len++] = current;
            output[output_len++] = current;
            output[output_len++] = (uint8_t)count;
            i = j;
        } else {
            size_t k = i;
            do {
                if (output_len >= output_max) {
                    return 0;
                }
                output[output_len++] = input[k];
                k++;
            } while (k < input_len && k < i + count && input[k] != (k + 1 < input_len ? input[k + 1] : 0));
            i = k;
        }
    }
    
    return output_len;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }
    
    size_t output_len = 0;
    size_t i = 0;
    
    for (i = 0; i < input_len; ) {
        if (i + 2 < input_len && input[i] == input[i + 1]) {
            uint8_t value = input[i];
            uint8_t count = input[i + 2];
            
            if (output_len + count > output_max) {
                return 0;
            }
            
            size_t n;
            for (n = 0; n < count; n++) {
                output[output_len++] = value;
            }
            i += 3;
        } else {
            if (output_len >= output_max) {
                return 0;
            }
            output[output_len++] = input[i];
            i++;
        }
    }
    
    return output_len;
}

void print_buffer_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    size_t i;
    for (i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (i % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    printf("Enter text to compress (max %zu characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(temp_input);
    if (input_len > 0 && temp_input[input_len - 1] == '\n') {
        temp_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, input_len);
    input_buf.length = input_len;
    
    printf("Original data (%zu bytes):\n", input_buf.length);
    print_buffer_hex(input_buf.data, input_buf.length);
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, 
                                           compressed_buf.data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    compressed_buf.length = compressed_size;
    
    printf("Compressed data (%zu bytes):\n", compressed_buf.length);
    print_buffer_hex(compressed_buf.data, compressed_buf.length);
    
    size_t decompressed_size = simple_decompress(compressed_buf.data, compressed_buf.length,
                                                decompressed_buf.data, MAX_INPUT_SIZE);
    if (