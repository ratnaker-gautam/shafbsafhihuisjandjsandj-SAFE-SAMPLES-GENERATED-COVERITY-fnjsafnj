//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 16
#define VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

static int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0 || size > MAX_INPUT_SIZE) {
        return 0;
    }
    for (size_t i = 0; i < size; i++) {
        if (!VALID_CHAR(data[i])) {
            return 0;
        }
    }
    return 1;
}

static size_t compress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size > CHUNK_SIZE || input_size == 0) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        if (i + 1 < input_size) {
            uint8_t high = input[i] >> 4;
            uint8_t low = input[i + 1] >> 4;
            output[output_size++] = (high << 4) | low;
        } else {
            output[output_size++] = input[i] >> 4;
        }
    }
    return output_size;
}

static size_t decompress_chunk(const uint8_t *input, size_t input_size, uint8_t *output) {
    if (input_size == 0 || input_size > CHUNK_SIZE) {
        return 0;
    }
    size_t output_size = 0;
    for (size_t i = 0; i < input_size; i++) {
        uint8_t byte = input[i];
        output[output_size++] = (byte >> 4) << 4;
        if (output_size < CHUNK_SIZE) {
            output[output_size++] = (byte & 0x0F) << 4;
        }
    }
    return output_size;
}

static int compress_buffer(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE) {
        size_t chunk_size = (input->size - i) < CHUNK_SIZE ? (input->size - i) : CHUNK_SIZE;
        size_t compressed_size = compress_chunk(input->data + i, chunk_size, output->data + output->size);
        if (compressed_size == 0 || output->size + compressed_size > MAX_OUTPUT_SIZE) {
            return 0;
        }
        output->size += compressed_size;
    }
    return 1;
}

static int decompress_buffer(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return 0;
    }
    output->size = 0;
    for (size_t i = 0; i < input->size; i += CHUNK_SIZE / COMPRESSION_RATIO) {
        size_t chunk_size = (input->size - i) < (CHUNK_SIZE / COMPRESSION_RATIO) ? (input->size - i) : (CHUNK_SIZE / COMPRESSION_RATIO);
        size_t decompressed_size = decompress_chunk(input->data + i, chunk_size, output->data + output->size);
        if (decompressed_size == 0 || output->size + decompressed_size > MAX_INPUT_SIZE) {
            return 0;
        }
        output->size += decompressed_size;
    }
    return 1;
}

static void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->size == 0) {
        return;
    }
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
    }
    printf("\n");
}

static void print_buffer_text(const Buffer *buf) {
    if (buf == NULL || buf->size == 0) {
        return;
    }
    for (size_t i = 0; i < buf->size; i++) {
        printf("%c", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};
    
    char line[MAX_INPUT_SIZE + 2];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    if (len > MAX_INPUT_SIZE) {
        return EXIT_FAILURE;
    }
    memcpy(input.data, line, len);
    input.size = len;
    
    if (!validate_input(input.data, input.size)) {
        return EXIT_FAILURE;
    }
    
    if (!compress_buffer(&input, &compressed)) {