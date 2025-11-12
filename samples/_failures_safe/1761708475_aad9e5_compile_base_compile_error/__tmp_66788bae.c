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
        for (; j < input_len; j++) {
            if (input[j] == current && count < 255) {
                count++;
            } else {
                break;
            }
        }
        
        output[output_len++] = current;
        output[output_len++] = (uint8_t)count;
        
        i = j;
    }
    
    return output_len;
}

size_t simple_decompress(const uint8_t* input, size_t input_len, uint8_t* output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }

    if (input_len % 2 != 0) {
        return 0;
    }

    size_t output_len = 0;
    size_t i = 0;
    
    do {
        if (i >= input_len) break;
        
        uint8_t character = input[i];
        uint8_t count = input[i + 1];
        
        if (output_len + count > output_max) {
            return 0;
        }
        
        size_t k = 0;
        while (k < count) {
            output[output_len++] = character;
            k++;
        }
        
        i += 2;
    } while (i < input_len);
    
    return output_len;
}

void print_buffer_hex(const uint8_t* data, size_t len) {
    if (data == NULL || len == 0) return;
    
    for (size_t idx = 0; idx < len; idx++) {
        printf("%02X", data[idx]);
        if (idx < len - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    Buffer input_buf;
    Buffer compressed_buf;
    Buffer decompressed_buf;
    
    initialize_buffer(&input_buf);
    initialize_buffer(&compressed_buf);
    initialize_buffer(&decompressed_buf);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
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
    
    if (input_len > MAX_INPUT_SIZE - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, input_len);
    input_buf.length = input_len;
    
    size_t compressed_len = simple_compress(input_buf.data, input_buf.length, 
                                          compressed_buf.data, MAX_OUTPUT_SIZE);
    if (compressed_len == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    compressed_buf.length = compressed_len;
    
    size_t decompressed_len = simple_decompress(compressed_buf.data, compressed_buf.length,
                                               decompressed_buf.data, MAX_INPUT_SIZE);
    if (decompressed_len == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    decompressed_buf.length = decompressed_len;
    
    printf("Original: ");
    print_buffer_hex(input_buf.data, input_buf.length);
    
    printf("Compressed: ");
    print_buffer_hex(compressed_buf.data, compressed_buf.length);
    
    printf("Decompressed: ");
    print_buffer_hex(decompressed_buf.data, decompressed_buf.length);
    
    if (input_len > 0) {
        printf("Compression ratio: %.2f%%\n", 
               (1.0 - (double)compressed_len / (double)input_len) * 100.0);
    } else {
        printf("Compression ratio: 0.00%%\n");