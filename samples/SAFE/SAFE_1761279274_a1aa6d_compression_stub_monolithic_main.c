//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

int compress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    if (input->size > MAX_INPUT_SIZE) {
        return -1;
    }

    size_t out_idx = 0;
    size_t in_idx = 0;

    while (in_idx < input->size) {
        if (out_idx + 2 >= MAX_OUTPUT_SIZE) {
            return -1;
        }

        uint8_t current = input->data[in_idx];
        size_t count = 1;

        while (in_idx + count < input->size && 
               input->data[in_idx + count] == current && 
               count < 255) {
            count++;
        }

        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        in_idx += count;
    }

    output->size = out_idx;
    return 0;
}

int decompress_rle(const Buffer* input, Buffer* output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    if (input->size % 2 != 0) {
        return -1;
    }

    size_t out_idx = 0;
    size_t in_idx = 0;

    while (in_idx < input->size) {
        if (in_idx + 1 >= input->size) {
            return -1;
        }

        uint8_t count = input->data[in_idx++];
        uint8_t value = input->data[in_idx++];

        if (out_idx + count > MAX_INPUT_SIZE) {
            return -1;
        }

        for (uint8_t i = 0; i < count; i++) {
            output->data[out_idx++] = value;
        }
    }

    output->size = out_idx;
    return 0;
}

void print_buffer_hex(const Buffer* buf) {
    if (buf == NULL) return;
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X ", buf->data[i]);
    }
    printf("\n");
}

int main() {
    Buffer input = {0};
    Buffer compressed = {0};
    Buffer decompressed = {0};

    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE - 1);
    
    char temp_input[MAX_INPUT_SIZE];
    if (fgets(temp_input, sizeof(temp_input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }

    size_t len = strlen(temp_input);
    if (len > 0 && temp_input[len - 1] == '\n') {
        temp_input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }

    memcpy(input.data, temp_input, len);
    input.size = len;

    printf("Original data (%zu bytes): ", input.size);
    print_buffer_hex(&input);

    if (compress_rle(&input, &compressed) != 0) {
        printf("Compression failed\n");
        return 1;
    }

    printf("Compressed data (%zu bytes): ", compressed.size);
    print_buffer_hex(&compressed);

    if (decompress_rle(&compressed, &decompressed) != 0) {
        printf("Decompression failed\n");
        return 1;
    }

    printf("Decompressed data (%zu bytes): ", decompressed.size);
    print_buffer_hex(&decompressed);

    if (input.size != decompressed.size || 
        memcmp(input.data, decompressed.data, input.size) != 0) {
        printf("Verification failed\n");
        return 1;
    }

    printf("Compression and decompression successful\n");
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)compressed.size / input.size) * 100.0);

    return 0;
}