//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define COMPRESSED_MULTIPLIER 2
#define BYTE_MASK 0xFF
#define HIGH_NIBBLE_SHIFT 4
#define LOW_NIBBLE_MASK 0x0F
#define COMPRESSION_FLAG 0x80
#define RUN_LENGTH_MAX 127

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

#define BUFFER_INIT {NULL, 0, 0}
#define BUFFER_GROWTH_FACTOR 2
#define MIN_CAPACITY 16

static int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = MIN_CAPACITY;
    if (initial_capacity > SIZE_MAX / sizeof(uint8_t)) return 0;
    buf->data = malloc(initial_capacity * sizeof(uint8_t));
    if (!buf->data) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

static void buffer_free(Buffer *buf) {
    if (buf->data) free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

static int buffer_append(Buffer *buf, uint8_t value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity;
        if (buf->capacity > SIZE_MAX / BUFFER_GROWTH_FACTOR) return 0;
        new_capacity = buf->capacity * BUFFER_GROWTH_FACTOR;
        if (new_capacity < MIN_CAPACITY) new_capacity = MIN_CAPACITY;
        uint8_t *new_data = realloc(buf->data, new_capacity * sizeof(uint8_t));
        if (!new_data) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    buf->data[buf->size++] = value;
    return 1;
}

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && run_length <= RUN_LENGTH_MAX && 
               input[i + run_length] == current) {
            run_length++;
        }
        
        if (run_length > 1) {
            if (!buffer_append(output, COMPRESSION_FLAG | (run_length & RUN_LENGTH_MAX))) return 0;
            if (!buffer_append(output, current)) return 0;
            i += run_length;
        } else {
            if (!buffer_append(output, current)) return 0;
            i++;
        }
    }
    return 1;
}

static int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        
        if (current & COMPRESSION_FLAG) {
            size_t run_length = current & RUN_LENGTH_MAX;
            i++;
            if (i >= input_size) return 0;
            uint8_t value = input[i];
            
            for (size_t j = 0; j < run_length; j++) {
                if (!buffer_append(output, value)) return 0;
            }
            i++;
        } else {
            if (!buffer_append(output, current)) return 0;
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
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!buffer_init(&compressed, input_len * COMPRESSED_MULTIPLIER) ||
        !buffer_init(&decompressed, input_len * COMPRESSED_MULTIPLIER)) {
        printf("Memory allocation failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;