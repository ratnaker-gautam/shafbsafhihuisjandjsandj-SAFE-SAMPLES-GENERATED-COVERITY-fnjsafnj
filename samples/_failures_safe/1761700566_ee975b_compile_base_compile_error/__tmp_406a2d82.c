//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_THRESHOLD 4
#define RUN_LENGTH_MAX 255

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

#define BUFFER_INIT {NULL, 0, 0}
#define BUFFER_GROW(buffer, min_capacity) \
    do { \
        size_t new_capacity = (buffer).capacity ? (buffer).capacity * 2 : 64; \
        if (new_capacity < (min_capacity)) new_capacity = (min_capacity); \
        uint8_t *new_data = realloc((buffer).data, new_capacity); \
        if (!new_data) exit(EXIT_FAILURE); \
        (buffer).data = new_data; \
        (buffer).capacity = new_capacity; \
    } while (0)

#define BUFFER_APPEND(buffer, byte) \
    do { \
        if ((buffer).size >= (buffer).capacity) BUFFER_GROW(buffer, (buffer).size + 1); \
        (buffer).data[(buffer).size++] = (byte); \
    } while (0)

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size > MAX_INPUT_SIZE) return -1;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && count < RUN_LENGTH_MAX && input[i + count] == current) {
            count++;
        }
        
        if (count >= COMPRESSION_THRESHOLD) {
            BUFFER_APPEND(*output, current);
            BUFFER_APPEND(*output, (uint8_t)count);
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                BUFFER_APPEND(*output, current);
            }
            i += count;
        }
    }
    
    return 0;
}

static int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size > MAX_OUTPUT_SIZE) return -1;
    
    output->size = 0;
    size_t i = 0;
    
    while (i < input_size) {
        if (i + 1 < input_size && input[i] == input[i + 1]) {
            size_t j = i;
            while (j < input_size && input[j] == input[i] && (j - i) < RUN_LENGTH_MAX) {
                j++;
            }
            size_t count = j - i;
            if (count >= COMPRESSION_THRESHOLD) {
                if (i + 2 >= input_size) return -1;
                uint8_t value = input[i];
                uint8_t run_length = input[i + 1];
                if (run_length < COMPRESSION_THRESHOLD) return -1;
                
                for (size_t k = 0; k < run_length; k++) {
                    BUFFER_APPEND(*output, value);
                }
                i += 2;
            } else {
                for (size_t k = i; k < j; k++) {
                    BUFFER_APPEND(*output, input[k]);
                }
                i = j;
            }
        } else {
            BUFFER_APPEND(*output, input[i]);
            i++;
        }
    }
    
    return 0;
}

static void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input = BUFFER_INIT;
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    
    printf("Enter text to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    char line[MAX_INPUT_SIZE + 2];
    if (!fgets(line, sizeof(line), stdin)) {
        printf("Input error\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    if (len > MAX_INPUT_SIZE) {
        printf("Input too large\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < len; i++) {
        BUFFER_APPEND(input, (uint8_t)line[i]);
    }
    
    printf("\nOriginal data (%zu bytes):\n", input.size);
    print_hex(input.data, input.size);
    
    if (compress_rle(input.data, input.size, &compressed