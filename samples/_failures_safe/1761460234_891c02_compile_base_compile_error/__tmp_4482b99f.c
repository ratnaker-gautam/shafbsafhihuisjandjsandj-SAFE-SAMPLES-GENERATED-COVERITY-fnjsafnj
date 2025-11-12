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
#define BUFFER_CHECK(b, needed) \
    if ((b)->size + (needed) > (b)->capacity) { \
        size_t new_cap = (b)->capacity ? (b)->capacity * BUFFER_GROW_FACTOR : 64; \
        while (new_cap < (b)->size + (needed)) new_cap *= BUFFER_GROW_FACTOR; \
        uint8_t *new_data = realloc((b)->data, new_cap); \
        if (!new_data) return 0; \
        (b)->data = new_data; \
        (b)->capacity = new_cap; \
    }

static int buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (!buf || !data || len == 0 || len > SIZE_MAX - buf->size) return 0;
    BUFFER_CHECK(buf, len);
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

static int buffer_append_byte(Buffer *buf, uint8_t byte) {
    return buffer_append(buf, &byte, 1);
}

static void buffer_free(Buffer *buf) {
    if (buf) {
        free(buf->data);
        buf->data = NULL;
        buf->size = buf->capacity = 0;
    }
}

static int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size == 0 || input_size > MAX_INPUT_SIZE) return 0;
    
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
            if (!buffer_append_byte(output, COMPRESSED_MARKER) ||
                !buffer_append_byte(output, current) ||
                !buffer_append_byte(output, (uint8_t)run_length)) {
                return 0;
            }
            i += run_length;
        } else {
            if (!buffer_append_byte(output, current)) return 0;
            i++;
        }
    }
    
    return 1;
}

static int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (!input || !output || input_size == 0) return 0;
    
    output->size = 0;
    
    size_t i = 0;
    while (i < input_size) {
        if (input[i] == COMPRESSED_MARKER) {
            if (i + 2 >= input_size) return 0;
            
            uint8_t value = input[i + 1];
            uint8_t count = input[i + 2];
            
            if (count < MIN_RUN_LENGTH || count > MAX_RUN_LENGTH) return 0;
            
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append_byte(output, value)) return 0;
            }
            
            i += 3;
        } else {
            if (!buffer_append_byte(output, input[i])) return 0;
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
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (max %d bytes as hex, empty line to finish):\n", MAX_INPUT_SIZE);
    
    char line[256];
    while (input_size < MAX_INPUT_SIZE && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        char *ptr = line;
        while (*ptr && input_size < MAX_INPUT_SIZE) {
            while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') ptr++;
            if (!*ptr) break;
            
            if (sscanf(ptr, "%2hhx", &input_data[input_size]) != 1) {
                printf("Invalid hex input\n");
                return 1;
            }
            input_size++;
            ptr += 2;
        }
    }
    
    if (input_size == 0)