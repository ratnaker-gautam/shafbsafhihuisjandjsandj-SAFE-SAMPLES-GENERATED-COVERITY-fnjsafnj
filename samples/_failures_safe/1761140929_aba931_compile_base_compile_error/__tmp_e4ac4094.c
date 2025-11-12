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
#define BUFFER_GROW_FACTOR 2
#define BUFFER_MIN_CAPACITY 16

static int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = BUFFER_MIN_CAPACITY;
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
    buf->size = buf->capacity = 0;
}

static int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (!buf || !data || data_size == 0) return 0;
    
    if (buf->size > SIZE_MAX - data_size) return 0;
    size_t new_size = buf->size + data_size;
    
    if (new_size > buf->capacity) {
        size_t new_capacity = buf->capacity;
        while (new_capacity < new_size) {
            if (new_capacity > SIZE_MAX / BUFFER_GROW_FACTOR) return 0;
            new_capacity *= BUFFER_GROW_FACTOR;
        }
        
        uint8_t *new_data = realloc(buf->data, new_capacity * sizeof(uint8_t));
        if (!new_data) return 0;
        
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    
    memcpy(buf->data + buf->size, data, data_size);
    buf->size = new_size;
    return 1;
}

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = input[i];
        size_t run_length = 1;
        
        while (i + run_length < input_size && 
               input[i + run_length] == current && 
               run_length < MAX_RUN_LENGTH) {
            run_length++;
        }
        
        if (run_length >= MIN_RUN_LENGTH) {
            uint8_t compressed[3] = {COMPRESSED_MARKER, current, (uint8_t)run_length};
            if (!buffer_append(output, compressed, 3)) return 0;
            i += run_length;
        } else {
            if (!buffer_append(output, &current, 1)) return 0;
            i++;
        }
    }
    
    return 1;
}

static int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || input_size == 0 || !output) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER && i + 2 < input_size) {
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            i += 3;
        } else {
            if (!buffer_append(output, &input[i], 1)) return 0;
            i++;
        }
    }
    
    return 1;
}

static void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
        else if ((i + 1) % 8 == 0) printf("  ");
        else printf(" ");
    }
    if (size % 16 != 0) printf("\n");
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (!fgets(input_str, sizeof(input_str), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    Buffer compressed = BUFFER_INIT;
    Buffer decompressed = BUFFER_INIT;