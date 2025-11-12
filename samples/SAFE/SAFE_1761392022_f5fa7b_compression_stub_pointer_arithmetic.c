//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0 || initial_capacity > MAX_OUTPUT_SIZE) {
        initial_capacity = 64;
    }
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        exit(EXIT_FAILURE);
    }
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, unsigned char value) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity > MAX_OUTPUT_SIZE) {
            new_capacity = MAX_OUTPUT_SIZE;
        }
        if (new_capacity <= buf->capacity) {
            return;
        }
        unsigned char *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) {
            return;
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    if (buf->data != NULL) {
        free(buf->data);
        buf->data = NULL;
    }
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }
    
    const unsigned char *current = input;
    const unsigned char *end = input + input_size;
    
    while (current < end) {
        unsigned char value = *current;
        size_t count = 1;
        
        const unsigned char *next = current + 1;
        while (next < end && *next == value && count < UCHAR_MAX) {
            count++;
            next++;
        }
        
        if (count > 3 || value == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, value);
            buffer_append(output, (unsigned char)count);
        } else {
            for (size_t i = 0; i < count; i++) {
                buffer_append(output, value);
            }
        }
        
        current += count;
    }
    
    return 0;
}

int main(void) {
    unsigned char input[MAX_INPUT_SIZE];
    size_t input_size = 0;
    
    printf("Enter input size (1-%d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_size) != 1) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    if (input_size == 0 || input_size > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %zu bytes (0-255): ", input_size);
    for (size_t i = 0; i < input_size; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid input value\n");
            return EXIT_FAILURE;
        }
        if (value < 0 || value > 255) {
            printf("Invalid input value\n");
            return EXIT_FAILURE;
        }
        *(input + i) = (unsigned char)value;
    }
    
    Buffer output;
    buffer_init(&output, 64);
    
    if (compress_rle(input, input_size, &output) != 0) {
        printf("Compression failed\n");
        buffer_free(&output);
        return EXIT_FAILURE;
    }
    
    printf("Original size: %zu\n", input_size);
    printf("Compressed size: %zu\n", output.size);
    printf("Compression ratio: %.2f\n", (double)output.size / input_size);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output.size; i++) {
        printf("%d ", *(output.data + i));
    }
    printf("\n");
    
    buffer_free(&output);
    return EXIT_SUCCESS;
}