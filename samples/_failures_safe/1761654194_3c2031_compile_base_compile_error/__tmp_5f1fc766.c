//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_CHUNK 8
#define MAX_OUTPUT_SIZE (MAX_INPUT_SIZE * 2)

#define RLE_THRESHOLD 3
#define BYTE_MASK 0xFF
#define SAFE_ADD(a, b) (((a) > SIZE_MAX - (b)) ? SIZE_MAX : ((a) + (b)))

typedef struct {
    unsigned char data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const unsigned char *input, size_t len) {
    return (input != NULL) && (len > 0) && (len <= MAX_INPUT_SIZE);
}

static size_t simple_rle_compress(const unsigned char *input, size_t len, Buffer *output) {
    if (!validate_input(input, len) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        if (out_idx >= MAX_OUTPUT_SIZE - 2) return 0;
        
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < len && count < RLE_THRESHOLD && input[i + count] == current) {
            count++;
        }
        
        if (count >= RLE_THRESHOLD) {
            output->data[out_idx++] = current;
            output->data[out_idx++] = (unsigned char)count;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                if (out_idx >= MAX_OUTPUT_SIZE) return 0;
                output->data[out_idx++] = input[i + j];
            }
            i += count;
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static size_t simple_rle_decompress(const unsigned char *input, size_t len, Buffer *output) {
    if (!validate_input(input, len) || output == NULL) return 0;
    
    size_t out_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        if (out_idx >= MAX_INPUT_SIZE) return 0;
        
        if (i + 1 < len && input[i] == input[i + 1]) {
            size_t run_length = 1;
            while (i + run_length < len && input[i] == input[i + run_length] && run_length < RLE_THRESHOLD) {
                run_length++;
            }
            
            if (run_length >= RLE_THRESHOLD) {
                if (i + run_length + 1 >= len) return 0;
                unsigned char count = input[i + run_length + 1];
                if (count < RLE_THRESHOLD) return 0;
                
                size_t new_size = SAFE_ADD(out_idx, count);
                if (new_size > MAX_INPUT_SIZE) return 0;
                
                for (size_t j = 0; j < count; j++) {
                    output->data[out_idx++] = input[i];
                }
                i += run_length + 2;
            } else {
                output->data[out_idx++] = input[i++];
            }
        } else {
            output->data[out_idx++] = input[i++];
        }
    }
    
    output->size = out_idx;
    return out_idx;
}

static void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02x", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter data to compress (max %d bytes as hex digits): ", MAX_INPUT_SIZE);
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    
    if (fgets(hex_input, sizeof(hex_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[--hex_len] = '\0';
    }
    
    if (hex_len % 2 != 0 || hex_len > MAX_INPUT_SIZE * 2) {
        fprintf(stderr, "Invalid hex input length\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = hex_len / 2;
    for (size_t i = 0; i < input_len; i++) {
        char hex_byte[3] = {hex_input[i * 2], hex_input[i * 2 + 1], '\0'};
        char *endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) {
            fprintf(stderr, "Invalid hex digit\n");
            return EXIT_FAILURE;
        }
        input_data[i] = (unsigned char)value;
    }
    
    if (simple_rle_compress(input_data, input_len, &compressed) == 0) {
        fprintf(stderr, "