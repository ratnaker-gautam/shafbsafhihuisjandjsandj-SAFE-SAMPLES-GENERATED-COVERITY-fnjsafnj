//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define MAX_COMPRESSED_SIZE (MAX_INPUT_SIZE * COMPRESSED_MULTIPLIER)
#define RUN_LENGTH_MAX 255
#define MIN_COMPRESSIBLE_RUN 3

typedef struct {
    uint8_t data[MAX_COMPRESSED_SIZE];
    size_t size;
} CompressedData;

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} InputData;

static int read_input(InputData* input) {
    if (!input) return 0;
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t total_read = 0;
    int c;
    while (total_read < MAX_INPUT_SIZE && (c = getchar()) != EOF && c != '\n') {
        input->data[total_read++] = (uint8_t)c;
    }
    
    if (total_read == 0) {
        printf("No input received\n");
        return 0;
    }
    
    input->size = total_read;
    return 1;
}

static size_t compress_rle(const InputData* input, CompressedData* output) {
    if (!input || !output || input->size == 0) return 0;
    
    size_t in_pos = 0;
    size_t out_pos = 0;
    
    while (in_pos < input->size && out_pos < MAX_COMPRESSED_SIZE - 2) {
        uint8_t current = input->data[in_pos];
        size_t run_length = 1;
        
        while (in_pos + run_length < input->size && 
               input->data[in_pos + run_length] == current && 
               run_length < RUN_LENGTH_MAX) {
            run_length++;
        }
        
        if (run_length >= MIN_COMPRESSIBLE_RUN) {
            if (out_pos + 2 >= MAX_COMPRESSED_SIZE) break;
            output->data[out_pos++] = current;
            output->data[out_pos++] = (uint8_t)run_length;
            in_pos += run_length;
        } else {
            if (out_pos + 1 >= MAX_COMPRESSED_SIZE) break;
            output->data[out_pos++] = current;
            in_pos++;
        }
    }
    
    output->size = out_pos;
    return out_pos;
}

static void display_compression_info(const InputData* input, const CompressedData* output) {
    if (!input || !output) return;
    
    printf("Original size: %zu bytes\n", input->size);
    printf("Compressed size: %zu bytes\n", output->size);
    
    if (input->size > 0) {
        double ratio = (double)output->size / (double)input->size * 100.0;
        printf("Compression ratio: %.1f%%\n", ratio);
    }
    
    printf("Compressed data (hex): ");
    for (size_t i = 0; i < output->size && i < 32; i++) {
        printf("%02x ", output->data[i]);
    }
    if (output->size > 32) printf("...");
    printf("\n");
}

int main(void) {
    InputData input = {0};
    CompressedData compressed = {0};
    
    if (!read_input(&input)) {
        return EXIT_FAILURE;
    }
    
    if (compress_rle(&input, &compressed) == 0) {
        printf("Compression failed\n");
        return EXIT_FAILURE;
    }
    
    display_compression_info(&input, &compressed);
    
    return EXIT_SUCCESS;
}