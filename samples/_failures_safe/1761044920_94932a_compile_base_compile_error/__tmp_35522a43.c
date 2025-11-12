//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048
#define COMPRESSION_RATIO 2
#define CHUNK_SIZE 8
#define MASK_BYTE 0xAA

#define CHECK_NULL(ptr) if ((ptr) == NULL) { return EXIT_FAILURE; }
#define CHECK_BOUNDS(len, max) if ((len) > (max) || (len) < 0) { return EXIT_FAILURE; }
#define SAFE_ADD(a, b) (((b) > 0) && ((a) > INT_MAX - (b))) ? INT_MAX : ((a) + (b))

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t length;
} Buffer;

static int compress_chunk(const uint8_t *input, uint8_t *output, size_t len) {
    if (input == NULL || output == NULL || len == 0 || len > CHUNK_SIZE) {
        return -1;
    }
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ MASK_BYTE;
    }
    return 0;
}

static int decompress_chunk(const uint8_t *input, uint8_t *output, size_t len) {
    if (input == NULL || output == NULL || len == 0 || len > CHUNK_SIZE) {
        return -1;
    }
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ MASK_BYTE;
    }
    return 0;
}

static int compress_buffer(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->length > MAX_INPUT_SIZE) {
        return -1;
    }
    output->length = 0;
    size_t remaining = input->length;
    const uint8_t *in_ptr = input->data;
    uint8_t *out_ptr = output->data;

    while (remaining > 0) {
        size_t chunk_len = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        if (compress_chunk(in_ptr, out_ptr, chunk_len) != 0) {
            return -1;
        }
        in_ptr += chunk_len;
        out_ptr += chunk_len;
        int new_len = SAFE_ADD(output->length, chunk_len);
        if (new_len == INT_MAX || new_len > MAX_OUTPUT_SIZE) {
            return -1;
        }
        output->length = new_len;
        remaining -= chunk_len;
    }
    return 0;
}

static int decompress_buffer(const Buffer *input, Buffer *output) {
    if (input == NULL || output == NULL || input->length > MAX_INPUT_SIZE) {
        return -1;
    }
    output->length = 0;
    size_t remaining = input->length;
    const uint8_t *in_ptr = input->data;
    uint8_t *out_ptr = output->data;

    while (remaining > 0) {
        size_t chunk_len = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;
        if (decompress_chunk(in_ptr, out_ptr, chunk_len) != 0) {
            return -1;
        }
        in_ptr += chunk_len;
        out_ptr += chunk_len;
        int new_len = SAFE_ADD(output->length, chunk_len);
        if (new_len == INT_MAX || new_len > MAX_OUTPUT_SIZE) {
            return -1;
        }
        output->length = new_len;
        remaining -= chunk_len;
    }
    return 0;
}

static int read_input(Buffer *buf) {
    if (buf == NULL) {
        return -1;
    }
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    char temp[MAX_INPUT_SIZE * 2];
    if (fgets(temp, sizeof(temp), stdin) == NULL) {
        return -1;
    }
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == '\n') {
        temp[len - 1] = '\0';
        len--;
    }
    if (len > MAX_INPUT_SIZE) {
        return -1;
    }
    memcpy(buf->data, temp, len);
    buf->length = len;
    return 0;
}

static void print_buffer(const Buffer *buf) {
    if (buf == NULL) {
        return;
    }
    for (size_t i = 0; i < buf->length; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main(void) {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};

    if (read_input(&input) != 0) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }

    printf("Original data (%zu bytes): ", input.length);
    print_buffer(&input);

    if (compress_buffer(&input, &compressed) != 0) {
        fprintf(stderr, "Compression error\n");
        return EXIT_FAILURE;
    }

    printf("Compressed data (%zu bytes): ", compressed.length);
    print_buffer(&compressed);

    if (decompress_buffer(&compressed, &decompressed) != 0) {
        fprintf(stderr