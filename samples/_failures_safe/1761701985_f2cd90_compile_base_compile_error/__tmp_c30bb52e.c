//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

int buffer_init(Buffer *buf, size_t initial_capacity) {
    if (buf == NULL || initial_capacity == 0) return 0;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) return 0;
    buf->size = 0;
    buf->capacity = initial_capacity;
    return 1;
}

void buffer_free(Buffer *buf) {
    if (buf != NULL && buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

int buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (buf == NULL || src == NULL || len == 0) return 0;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->capacity) return 0;
        if (buf->size + len > new_capacity) {
            new_capacity = buf->size + len;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        uint8_t current = *(input + i);
        size_t count = 1;
        
        while (i + count < input_len && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        
        if (count > 1) {
            uint8_t run[2] = {current, (uint8_t)count};
            if (!buffer_append(output, run, 2)) return 0;
            i += count;
        } else {
            size_t literal_start = i;
            while (i < input_len && (i + 1 >= input_len || *(input + i) != *(input + i + 1)) && (i - literal_start) < UINT8_MAX) {
                i++;
            }
            size_t literal_len = i - literal_start;
            if (literal_len > 0) {
                uint8_t marker = 0;
                if (!buffer_append(output, &marker, 1)) return 0;
                uint8_t len_byte = (uint8_t)literal_len;
                if (!buffer_append(output, &len_byte, 1)) return 0;
                if (!buffer_append(output, input + literal_start, literal_len)) return 0;
            }
        }
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) return 0;
    
    size_t i = 0;
    while (i < input_len) {
        if (i + 1 >= input_len) return 0;
        
        uint8_t marker = *(input + i);
        uint8_t count = *(input + i + 1);
        
        if (marker == 0) {
            if (i + 2 + count > input_len) return 0;
            if (!buffer_append(output, input + i + 2, count)) return 0;
            i += 2 + count;
        } else {
            if (count == 0) return 0;
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append(output, &marker, 1)) return 0;
            }
            i += 2;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_len = 0;
    
    printf("Enter input data (up to %d bytes): ", MAX_INPUT_SIZE);
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_len < MAX_INPUT_SIZE) {
        char *endptr;
        long value = strtol(token, &endptr, 10);
        if (*endptr != '\0' || value < 0 || value > 255) {
            fprintf(stderr, "Invalid input: must be space-separated bytes (0-255)\n");
            return 1;
        }
        input[input_len++] = (uint8_t)value;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_len == 0) {
        fprintf(stderr, "