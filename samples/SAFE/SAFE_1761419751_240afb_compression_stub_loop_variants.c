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
        for (; j < input_len && j < i + 255; j++) {
            if (input[j] == current) {
                count++;
            } else {
                break;
            }
        }
        
        if (count > 3) {
            output[output_len++] = 0xFF;
            output[output_len++] = current;
            output[output_len++] = (uint8_t)count;
            i += count;
        } else {
            size_t k;
            for (k = 0; k < count; k++) {
                if (output_len >= output_max) {
                    return 0;
                }
                output[output_len++] = current;
            }
            i += count;
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
    
    do {
        if (i >= input_len) break;
        
        if (input[i] == 0xFF && i + 2 < input_len) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (output_len + count > output_max) {
                return 0;
            }
            
            size_t j;
            for (j = 0; j < count; j++) {
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
    } while (i < input_len);
    
    return output_len;
}

int buffers_equal(const uint8_t* buf1, const uint8_t* buf2, size_t len) {
    if (buf1 == NULL || buf2 == NULL) {
        return 0;
    }
    
    size_t idx;
    for (idx = 0; idx < len; idx++) {
        if (buf1[idx] != buf2[idx]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    Buffer original, compressed, decompressed;
    initialize_buffer(&original);
    initialize_buffer(&compressed);
    initialize_buffer(&decompressed);
    
    uint8_t test_data[] = "AAAABBBCCCDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ";
    size_t test_len = strlen((char*)test_data);
    
    if (test_len > MAX_INPUT_SIZE) {
        fprintf(stderr, "Test data too large\n");
        return 1;
    }
    
    memcpy(original.data, test_data, test_len);
    original.length = test_len;
    
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
    
    if (decompressed.length != original.length) {
        fprintf(stderr, "Length mismatch: %zu vs %zu\n", decompressed.length, original.length);
        return 1;
    }
    
    if (!buffers_equal(original.data, decompressed.data, original.length)) {
        fprintf(stderr, "Data mismatch after decompression\n");
        return 1;
    }
    
    printf("Compression test successful\n");
    printf("Original size: %zu, Compressed size: %zu\n", original.length, compressed.length);
    
    return 0;
}