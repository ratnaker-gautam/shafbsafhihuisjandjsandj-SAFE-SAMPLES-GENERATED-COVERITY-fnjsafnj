//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    return 1;
}

size_t simple_compress(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return 0;
    }
    
    size_t output_index = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        if (count > 3 || current == 0xFF) {
            if (output_index + 2 >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            output[output_index++] = 0xFF;
            output[output_index++] = current;
            output[output_index++] = (uint8_t)count;
        } else {
            if (output_index + count >= MAX_OUTPUT_SIZE) {
                return 0;
            }
            for (size_t j = 0; j < count; j++) {
                output[output_index++] = current;
            }
        }
        
        i += count;
    }
    
    return output_index;
}

size_t read_input(Buffer *buf) {
    if (buf == NULL) {
        return 0;
    }
    
    printf("Enter data to compress (up to %zu bytes): ", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    int c;
    
    while ((c = getchar()) != EOF && total_read < MAX_INPUT_SIZE) {
        if (c == '\n') {
            break;
        }
        buf->data[total_read++] = (uint8_t)c;
    }
    
    buf->size = total_read;
    return total_read;
}

void print_hex(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) {
        return;
    }
    
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    Buffer input_buf = {0};
    uint8_t output_buf[MAX_OUTPUT_SIZE] = {0};
    
    if (read_input(&input_buf) == 0) {
        fprintf(stderr, "Error: No input data received\n");
        return 1;
    }
    
    if (!validate_input(input_buf.data, input_buf.size)) {
        fprintf(stderr, "Error: Invalid input data\n");
        return 1;
    }
    
    printf("Original data (%zu bytes):\n", input_buf.size);
    print_hex(input_buf.data, input_buf.size);
    
    size_t compressed_size = simple_compress(input_buf.data, input_buf.size, output_buf);
    
    if (compressed_size == 0) {
        fprintf(stderr, "Error: Compression failed\n");
        return 1;
    }
    
    printf("Compressed data (%zu bytes):\n", compressed_size);
    print_hex(output_buf, compressed_size);
    
    double ratio = (double)compressed_size / input_buf.size * 100;
    printf("Compression ratio: %.1f%%\n", ratio);
    
    return 0;
}