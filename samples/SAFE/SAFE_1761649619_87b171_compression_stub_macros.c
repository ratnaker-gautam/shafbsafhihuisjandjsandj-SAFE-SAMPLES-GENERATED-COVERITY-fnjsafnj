//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 4
#define RUN_LENGTH_MASK 0x7F
#define MAX_RUN_LENGTH 127

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t* input, size_t input_size) {
    if (input == NULL) return 0;
    if (input_size == 0) return 0;
    if (input_size > MAX_INPUT_SIZE) return 0;
    return 1;
}

static void initialize_buffer(Buffer* buf) {
    memset(buf->data, 0, sizeof(buf->data));
    buf->size = 0;
}

static int compress_run_length(const uint8_t* input, size_t input_size, Buffer* output) {
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && 
               input[i + run_length] == current && 
               run_length <= MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= COMPRESSION_THRESHOLD) {
            if (output->size + 2 > MAX_OUTPUT_SIZE) return 0;
            output->data[output->size++] = (uint8_t)(run_length | 0x80);
            output->data[output->size++] = current;
            i += run_length;
        } else {
            if (output->size + run_length > MAX_OUTPUT_SIZE) return 0;
            for (size_t j = 0; j < run_length; j++) {
                output->data[output->size++] = current;
            }
            i += run_length;
        }
    }
    return 1;
}

static void print_hex(const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    char input_line[MAX_INPUT_SIZE * 3];
    size_t input_size = 0;
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char* token = strtok(input_line, " \n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        unsigned long value;
        char* endptr;
        value = strtoul(token, &endptr, 16);
        if (endptr == token || *endptr != '\0' || value > UINT8_MAX) {
            fprintf(stderr, "Invalid hex byte: %s\n", token);
            return EXIT_FAILURE;
        }
        input_data[input_size++] = (uint8_t)value;
        token = strtok(NULL, " \n");
    }
    
    if (!validate_input(input_data, input_size)) {
        fprintf(stderr, "Invalid input data\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed;
    initialize_buffer(&compressed);
    
    if (!compress_run_length(input_data, input_size, &compressed)) {
        fprintf(stderr, "Compression failed - output buffer exceeded\n");
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", compressed.size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input_size) * 100.0);
    printf("Compressed data: ");
    print_hex(compressed.data, compressed.size);
    
    return EXIT_SUCCESS;
}