//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 8192
#define MAX_OUTPUT_SIZE 16384

typedef struct {
    uint8_t data[MAX_OUTPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }

    output->size = 0;
    size_t i = 0;

    while (i < input_size && output->size < MAX_OUTPUT_SIZE - 2) {
        uint8_t current = input[i];
        size_t count = 1;

        while (i + count < input_size && count < 255 && input[i + count] == current) {
            count++;
        }

        if (output->size + 2 > MAX_OUTPUT_SIZE) {
            return -1;
        }

        output->data[output->size++] = (uint8_t)count;
        output->data[output->size++] = current;
        i += count;
    }

    return 0;
}

int decompress_rle(const uint8_t* input, size_t input_size, Buffer* output) {
    if (input == NULL || output == NULL || input_size == 0 || input_size % 2 != 0) {
        return -1;
    }

    output->size = 0;
    size_t i = 0;

    while (i < input_size && output->size < MAX_OUTPUT_SIZE) {
        uint8_t count = input[i];
        uint8_t value = input[i + 1];

        if (output->size + count > MAX_OUTPUT_SIZE) {
            return -1;
        }

        for (uint8_t j = 0; j < count; j++) {
            output->data[output->size++] = value;
        }

        i += 2;
    }

    return 0;
}

int read_input(Buffer* buf) {
    if (buf == NULL) {
        return -1;
    }

    buf->size = 0;
    int c;
    
    while ((c = getchar()) != EOF && buf->size < MAX_INPUT_SIZE) {
        buf->data[buf->size++] = (uint8_t)c;
    }

    if (buf->size == 0) {
        return -1;
    }

    return 0;
}

void write_output(const Buffer* buf) {
    if (buf == NULL || buf->size == 0) {
        return;
    }

    for (size_t i = 0; i < buf->size; i++) {
        putchar(buf->data[i]);
    }
}

int main(void) {
    printf("Enter data to compress (end with Ctrl+D): ");
    fflush(stdout);

    Buffer input;
    if (read_input(&input) != 0) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    Buffer compressed;
    if (compress_rle(input.data, input.size, &compressed) != 0) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }

    printf("Compressed data (%zu bytes): ", compressed.size);
    write_output(&compressed);
    printf("\n");

    Buffer decompressed;
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        fprintf(stderr, "Decompression failed\n");
        return 1;
    }

    if (input.size != decompressed.size || memcmp(input.data, decompressed.data, input.size) != 0) {
        fprintf(stderr, "Verification failed\n");
        return 1;
    }

    printf("Decompression verified successfully\n");
    return 0;
}