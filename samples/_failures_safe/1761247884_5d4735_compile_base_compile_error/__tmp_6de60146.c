//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MARKER 0xFF
#define MAX_RUN_LENGTH 255
#define MIN_RUN_LENGTH 3

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

#define BUFFER_INIT {NULL, 0, 0}
#define BUFFER_GROW(buffer, min_capacity) do { \
    size_t new_capacity = (buffer).capacity * 2; \
    if (new_capacity < min_capacity) new_capacity = min_capacity; \
    if (new_capacity < 16) new_capacity = 16; \
    uint8_t *new_data = realloc((buffer).data, new_capacity); \
    if (!new_data) { perror("Memory allocation failed"); exit(EXIT_FAILURE); } \
    (buffer).data = new_data; \
    (buffer).capacity = new_capacity; \
} while(0)

#define BUFFER_APPEND(buffer, byte) do { \
    if ((buffer).size >= (buffer).capacity) { \
        BUFFER_GROW((buffer), (buffer).size + 1); \
    } \
    (buffer).data[(buffer).size++] = (byte); \
} while(0)

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    output->size = 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && run_length < MAX_RUN_LENGTH && 
               input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length >= MIN_RUN_LENGTH) {
            BUFFER_APPEND(*output, COMPRESSED_MARKER);
            BUFFER_APPEND(*output, current);
            BUFFER_APPEND(*output, (uint8_t)run_length);
            i += run_length;
        } else {
            BUFFER_APPEND(*output, current);
            i++;
        }
    }
    
    return 0;
}

static int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output) {
        return -1;
    }
    
    output->size = 0;
    
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_size) {
                return -1;
            }
            
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH) {
                return -1;
            }
            
            for (uint8_t j = 0; j < count; j++) {
                BUFFER_APPEND(*output, value);
            }
            
            i += 3;
        } else {
            BUFFER_APPEND(*output, input[i]);
            i++;
        }
    }
    
    return 0;
}

static void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    if (size % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    uint8_t original_data[MAX_INPUT_SIZE];
    size_t data_size = 0;
    
    printf("Enter data to compress (up to %d bytes as hex values, space separated): ", MAX_INPUT_SIZE);
    
    char line[4096];
    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *token = strtok(line, " \t\n");
    while (token && data_size < MAX_INPUT_SIZE) {
        unsigned long value;
        if (sscanf(token, "%02lx", &value) != 1 || value > 0xFF) {
            fprintf(stderr, "Invalid hex value: %s\n", token);
            return EXIT_FAILURE;
        }
        original_data[data_size++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (data_size == 0) {
        fprintf(stderr, "No valid input data provided\n");
        return EXIT_FAILURE;
    }
    
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    
    if (compress_rle(original_data, data_size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        free(compressed.data);
        return EXIT_FAILURE;
    }
    
    printf("Original data (%zu bytes):\n", data_size);
    print_hex(original_data, data_size);
    
    printf("Compressed data (%zu bytes):\n", compressed.size);
    print_hex(compressed.data, compressed.size