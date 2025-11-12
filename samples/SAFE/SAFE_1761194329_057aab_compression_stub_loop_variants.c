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

int validate_input(const uint8_t *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0) {
        return 0;
    }

    size_t output_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (output_idx + 2 >= output_max) {
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
        
        output[output_idx++] = current;
        output[output_idx++] = (uint8_t)count;
        
        i = j;
    }
    
    return output_idx;
}

size_t simple_decompress(const uint8_t *input, size_t input_len, uint8_t *output, size_t output_max) {
    if (!validate_input(input, input_len) || output == NULL || output_max == 0 || (input_len % 2) != 0) {
        return 0;
    }

    size_t output_idx = 0;
    size_t i = 0;
    
    do {
        if (i + 1 >= input_len) {
            break;
        }
        
        uint8_t value = input[i];
        uint8_t count = input[i + 1];
        
        if (output_idx + count > output_max) {
            return 0;
        }
        
        size_t k = 0;
        while (k < count) {
            output[output_idx++] = value;
            k++;
        }
        
        i += 2;
    } while (i < input_len);
    
    return output_idx;
}

void print_buffer_hex(const uint8_t *data, size_t len) {
    if (data == NULL || len == 0) {
        return;
    }
    
    for (size_t idx = 0; idx < len; idx++) {
        printf("%02x", data[idx]);
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
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char temp_input[MAX_INPUT_SIZE * 2];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t temp_len = strlen(temp_input);
    if (temp_len > 0 && temp_input[temp_len - 1] == '\n') {
        temp_input[temp_len - 1] = '\0';
        temp_len--;
    }
    
    if (temp_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Input too large\n");
        return 1;
    }
    
    memcpy(input_buf.data, temp_input, temp_len);
    input_buf.length = temp_len;
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.length, 
                                           compressed_buf.data, MAX_OUTPUT_SIZE);
    if (compressed_size == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    compressed_buf.length = compressed_size;
    
    printf("Compressed (%zu bytes): ", compressed_size);
    print_buffer_hex(compressed_buf.data, compressed_size);
    
    size_t decompressed_size = simple_decompress(compressed_buf.data, compressed_size,
                                                decompressed_buf.data, MAX_INPUT_SIZE);
    if (decompressed_size == 0 || decompressed_size != input_buf.length) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    decompressed_buf.length = decompressed_size;
    
    printf("Decompressed (%zu bytes): ", decompressed_size);
    for (size_t pos = 0; pos < decompressed_size; pos++) {
        putchar(decompressed_buf.data[pos]);
    }
    printf("\n");
    
    if (memcmp(input_buf.data, decompressed_buf.data, input_buf.length) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Compression and decompression successful\n");
    return 0;
}