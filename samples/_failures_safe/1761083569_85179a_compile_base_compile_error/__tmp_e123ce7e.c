//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

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
#define BUFFER_GROW(buffer, needed) \
    do { \
        size_t new_cap = (buffer).capacity == 0 ? 64 : (buffer).capacity * 2; \
        while (new_cap < (buffer).size + (needed)) new_cap *= 2; \
        uint8_t *new_data = realloc((buffer).data, new_cap); \
        if (!new_data) { \
            free((buffer).data); \
            exit(EXIT_FAILURE); \
        } \
        (buffer).data = new_data; \
        (buffer).capacity = new_cap; \
    } while(0)

#define BUFFER_APPEND(buffer, byte) \
    do { \
        if ((buffer).size >= (buffer).capacity) { \
            BUFFER_GROW(buffer, 1); \
        } \
        (buffer).data[(buffer).size++] = (byte); \
    } while(0)

static int buffer_init(Buffer *buf) {
    buf->data = malloc(64);
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = 64;
    return 1;
}

static void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = buf->capacity = 0;
}

static void compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
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
}

static int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_size) return 0;
            
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                BUFFER_APPEND(*output, value);
            }
            i += 3;
        } else {
            BUFFER_APPEND(*output, input[i]);
            i++;
        }
    }
    return 1;
}

static void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if (i + 1 < size) printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    Buffer input = BUFFER_INIT;
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    int success = 0;
    
    if (!buffer_init(&input) || !buffer_init(&compressed) || !buffer_init(&decompressed)) {
        fprintf(stderr, "Memory allocation failed\n");
        goto cleanup;
    }
    
    printf("Enter data to compress (max %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        char *pos = line;
        while (*pos && input.size < MAX_INPUT_SIZE) {
            if (*pos == ' ' || *pos == '\n' || *pos == '\r') {
                pos++;
                continue;
            }
            
            if (!isxdigit((unsigned char)pos[0]) || !isxdigit((unsigned char)pos[1])) {
                fprintf(stderr, "Invalid hex input\n");
                goto cleanup;
            }
            
            char hex[3] = {pos[0], pos[1], '\0'};
            char *endptr;
            unsigned long val = strtoul(hex, &endptr, 16);
            if (endptr == hex || val > UINT8_MAX) {
                fprintf(stderr, "Invalid hex value\n");
                goto cleanup;
            }
            uint8_t byte = (uint8_t)val;
            BUFFER_APPEND(input