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

int buffer_append(Buffer *buf, const uint8_t *data, size_t data_size) {
    if (buf == NULL || data == NULL || data_size == 0) return 0;
    if (buf->size + data_size > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + data_size) new_capacity = buf->size + data_size;
        if (new_capacity > SIZE_MAX) return 0;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) return 0;
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, data, data_size);
    buf->size += data_size;
    return 1;
}

int compress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    if (input_size > SIZE_MAX / 2) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        uint8_t current = *(input + i);
        size_t count = 1;
        while (i + count < input_size && count < UINT8_MAX && *(input + i + count) == current) {
            count++;
        }
        if (count >= 3) {
            uint8_t run[3] = {0xFF, current, (uint8_t)count};
            if (!buffer_append(output, run, 3)) return 0;
            i += count;
        } else {
            for (size_t j = 0; j < count; j++) {
                uint8_t literal[2] = {0x00, *(input + i + j)};
                if (!buffer_append(output, literal, 2)) return 0;
            }
            i += count;
        }
    }
    return 1;
}

int decompress_rle(const uint8_t *input, size_t input_size, Buffer *output) {
    if (input == NULL || input_size == 0 || output == NULL) return 0;
    
    size_t i = 0;
    while (i < input_size) {
        if (i + 1 >= input_size) return 0;
        uint8_t marker = *(input + i);
        uint8_t value = *(input + i + 1);
        
        if (marker == 0xFF) {
            if (i + 2 >= input_size) return 0;
            uint8_t count = *(input + i + 2);
            if (count == 0) return 0;
            for (uint8_t j = 0; j < count; j++) {
                if (!buffer_append(output, &value, 1)) return 0;
            }
            i += 3;
        } else if (marker == 0x00) {
            if (!buffer_append(output, &value, 1)) return 0;
            i += 2;
        } else {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    uint8_t input_data[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter data to compress (up to %d bytes): ", MAX_INPUT_SIZE);
    char line[MAX_INPUT_SIZE * 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *token = strtok(line, " \t\n");
    while (token != NULL && input_size < MAX_INPUT_SIZE) {
        char *endptr;
        long value = strtol(token, &endptr, 10);
        if (endptr == token || *endptr != '\0' || value < 0 || value > 255) {
            fprintf(stderr, "Invalid input: must be space-separated bytes (0-255)\n");
            return 1;
        }
        *(input_data + input_size) = (uint8_t)value;
        input_size++;
        token = strtok(NULL, " \t\n");
    }
    
    if (input_size == 0) {
        fprintf(stderr, "No valid input provided\n");
        return 1;
    }
    
    Buffer compressed;
    if (!buffer_init(&compressed, input_size * 2)) {
        fprintf(stderr, "Memory allocation failed\n