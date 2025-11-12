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
    size_t new_cap = (buffer).capacity ? (buffer).capacity * 2 : 64; \
    if (new_cap < (min_capacity)) new_cap = (min_capacity); \
    uint8_t *new_data = realloc((buffer).data, new_cap); \
    if (!new_data) { perror("Memory allocation failed"); exit(EXIT_FAILURE); } \
    (buffer).data = new_data; \
    (buffer).capacity = new_cap; \
} while(0)

#define BUFFER_APPEND(buffer, byte) do { \
    if ((buffer).size >= (buffer).capacity) { BUFFER_GROW(buffer, (buffer).size + 1); } \
    (buffer).data[(buffer).size++] = (byte); \
} while(0)

static int buffer_init(Buffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
    return 0;
}

static void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size > MAX_INPUT_SIZE) return -1;
    
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
    if (!input || !output) return -1;
    
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_size) return -1;
            
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH) return -1;
            
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
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input = BUFFER_INIT;
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    
    printf("Enter data to compress (max %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        char *token = strtok(line, " \t\n");
        while (token && input.size < MAX_INPUT_SIZE) {
            unsigned int byte;
            if (sscanf(token, "%02X", &byte) == 1 && byte <= 0xFF) {
                BUFFER_APPEND(input, (uint8_t)byte);
            }
            token = strtok(NULL, " \t\n");
        }
        
        if (input.size >= MAX_INPUT_SIZE) {
            printf("Input size limit reached.\n");
            break;
        }
    }
    
    if (input.size == 0) {
        printf("No input provided.\n");
        buffer_free(&input);
        return EXIT_FAILURE;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_hex(input.data, input.size);
    
    if (compress_rle(input.data, input.size, &compressed) != 0) {
        printf("Compression failed.\n");
        buffer_free(&input);
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }
    
    printf("\nCompressed data (%zu