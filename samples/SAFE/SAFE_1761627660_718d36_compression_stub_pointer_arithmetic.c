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
            free(buf->data);
            exit(EXIT_FAILURE);
        }
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    *(buf->data + buf->size) = value;
    buf->size++;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
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

        if (count > 2 || value == 0xFF) {
            buffer_append(output, 0xFF);
            buffer_append(output, (unsigned char)count);
            buffer_append(output, value);
        } else {
            for (size_t i = 0; i < count; i++) {
                buffer_append(output, value);
            }
        }

        current = next;
    }

    return 0;
}

int decompress_rle(const unsigned char *input, size_t input_size, Buffer *output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size > MAX_INPUT_SIZE) {
        return -1;
    }

    const unsigned char *current = input;
    const unsigned char *end = input + input_size;

    while (current < end) {
        if (*current == 0xFF) {
            if (current + 2 >= end) {
                return -1;
            }
            unsigned char count = *(current + 1);
            unsigned char value = *(current + 2);
            if (count == 0) {
                return -1;
            }
            for (size_t i = 0; i < count; i++) {
                buffer_append(output, value);
            }
            current += 3;
        } else {
            buffer_append(output, *current);
            current++;
        }
    }

    return 0;
}

void print_hex(const unsigned char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", *(data + i));
    }
    printf("\n");
}

int main() {
    unsigned char input_data[] = {0x01, 0x01, 0x01, 0x02, 0x03, 0x03, 0x04, 0x05, 0x05, 0x05};
    size_t input_size = sizeof(input_data);

    Buffer compressed;
    buffer_init(&compressed, 32);

    if (compress_rle(input_data, input_size, &compressed) != 0) {
        buffer_free(&compressed);
        return EXIT_FAILURE;
    }

    Buffer decompressed;
    buffer_init(&decompressed, 32);

    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return EXIT_FAILURE;
    }

    printf("Original: ");
    print_hex(input_data, input_size);
    printf("Compressed: ");
    print_hex(compressed.data, compressed.size);
    printf("Decompressed: ");
    print_hex(decompressed.data, decompressed.size);

    int match = (input_size == decompressed.size) && 
                (memcmp(input_data, decompressed.data, input_size) == 0);
    printf("Round-trip successful: %s\n", match ? "Yes" : "No");

    buffer_free(&compressed);
    buffer_free(&decompressed);

    return match ? EXIT_SUCCESS : EXIT_FAILURE;
}