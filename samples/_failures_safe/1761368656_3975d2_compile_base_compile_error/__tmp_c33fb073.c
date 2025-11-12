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

static void compress_rle(const unsigned char* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    output->size = 0;
    
    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        unsigned char current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < BYTE_MASK && 
               input[i + count] == current && count < RLE_THRESHOLD + BYTE_MASK) {
            count++;
        }
        
        if (count >= RLE_THRESHOLD) {
            if (output->size + 2 > MAX_OUTPUT_SIZE) break;
            output->data[output->size++] = current;
            output->data[output->size++] = (unsigned char)(count - RLE_THRESHOLD);
            i += count;
        } else {
            size_t literal_count = 0;
            while (i + literal_count < input_size && literal_count < count + 1 && 
                   literal_count < BYTE_MASK && output->size + literal_count + 1 < MAX_OUTPUT_SIZE) {
                if (i + literal_count + 1 < input_size && 
                    input[i + literal_count] == input[i + literal_count + 1] &&
                    literal_count + 2 < input_size && 
                    input[i + literal_count + 1] == input[i + literal_count + 2]) {
                    break;
                }
                literal_count++;
            }
            
            if (literal_count > 0) {
                if (output->size + literal_count + 1 > MAX_OUTPUT_SIZE) break;
                output->data[output->size++] = (unsigned char)(literal_count - 1);
                for (size_t j = 0; j < literal_count; j++) {
                    output->data[output->size++] = input[i + j];
                }
                i += literal_count;
            } else {
                if (output->size + 2 > MAX_OUTPUT_SIZE) break;
                output->data[output->size++] = current;
                output->data[output->size++] = 0;
                i++;
            }
        }
    }
}

static void decompress_rle(const Buffer* input, Buffer* output) {
    size_t i = 0;
    output->size = 0;
    
    while (i < input->size && output->size < MAX_INPUT_SIZE) {
        if (i + 1 >= input->size) break;
        
        unsigned char marker = input->data[i];
        
        if (marker < RLE_THRESHOLD) {
            size_t literal_count = marker + 1;
            if (i + literal_count + 1 > input->size) break;
            
            for (size_t j = 0; j < literal_count; j++) {
                if (output->size >= MAX_INPUT_SIZE) break;
                output->data[output->size++] = input->data[i + 1 + j];
            }
            i += literal_count + 1;
        } else {
            if (i + 1 >= input->size) break;
            unsigned char value = input->data[i];
            unsigned char count = input->data[i + 1] + RLE_THRESHOLD;
            
            if (SAFE_ADD(output->size, count) > MAX_INPUT_SIZE) break;
            
            for (unsigned char j = 0; j < count; j++) {
                output->data[output->size++] = value;
            }
            i += 2;
        }
    }
}

static int validate_input(const unsigned char* data, size_t size) {
    return data != NULL && size > 0 && size <= MAX_INPUT_SIZE;
}

int main(void) {
    unsigned char input_data[MAX_INPUT_SIZE];
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    if (fgets((char*)input_data, sizeof(input_data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_size = strlen((char*)input_data);
    if (input_size > 0 && input_data[input_size - 1] == '\n') {
        input_data[--input_size] = '\0';
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input_data);
    printf("Original size: %zu bytes\n", input_size);
    
    compress_rle(input_data, input_size, &compressed);
    printf("Compressed size: %zu bytes\n", compressed.size);
    
    if (compressed.size > 0) {
        double ratio = (input_size > 0) ? (100.0