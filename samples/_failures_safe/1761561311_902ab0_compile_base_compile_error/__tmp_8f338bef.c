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
        if (output_len + 3 >= output_max) {
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
            while (k < j) {
                if (output_len >= output_max) {
                    return 0;
                }
                output[output_len++] = input[k];
                k++;
            }
            i = j;
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
        if (output_len >= output_max) {
            return 0;
        }
        
        if (i + 2 < input_len && input[i] == input[i + 1]) {
            uint8_t count = input[i + 2];
            if (count >= 3) {
                size_t bytes_needed = (size_t)count;
                if (output_len + bytes_needed > output_max) {
                    return 0;
                }
                
                uint8_t value = input[i];
                size_t k = 0;
                do {
                    output[output_len++] = value;
                    k++;
                } while (k < count);
                
                i += 3;
                continue;
            }
        }
        
        output[output_len++] = input[i];
        i++;
    }
    
    return output_len;
}

int compare_buffers(const uint8_t* buf1, const uint8_t* buf2, size_t len) {
    if (buf1 == NULL || buf2 == NULL) {
        return 0;
    }
    
    size_t i = 0;
    while (i < len) {
        if (buf1[i] != buf2[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

int main(void) {
    Buffer original, compressed, decompressed;
    initialize_buffer(&original);
    initialize_buffer(&compressed);
    initialize_buffer(&decompressed);
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char input_line[MAX_INPUT_SIZE];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    memcpy(original.data, input_line, input_len);
    original.length = input_len;
    
    compressed.length = simple_compress(original.data, original.length, compressed.data, MAX_OUTPUT_SIZE);
    if (compressed.length == 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }
    
    decompressed.length = simple_decompress(compressed.data, compressed.length, decompressed.data, MAX_INPUT_SIZE);
    if (decompressed.length == 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }
    
    if (!compare_buffers(original.data, decompressed.data, original.length)) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }
    
    printf("Original size: %zu bytes\n", original.length);
    printf("Compressed size: %zu bytes\n", compressed.length);
    printf("Compression ratio: %.2f%%\n", (1.0 - (double)compressed.length / original.length) * 100.0);