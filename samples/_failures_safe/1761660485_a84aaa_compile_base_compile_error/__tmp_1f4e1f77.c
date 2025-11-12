//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMPRESS_NONE,
    COMPRESS_RLE,
    COMPRESS_LZW,
    COMPRESS_HUFFMAN
};

struct file_data {
    uint8_t *data;
    size_t size;
    size_t capacity;
};

int read_file_data(struct file_data *fd, const char *input) {
    if (fd == NULL || input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0) return 0;
    if (len > 1024) len = 1024;
    fd->data = malloc(len);
    if (fd->data == NULL) return 0;
    memcpy(fd->data, input, len);
    fd->size = len;
    fd->capacity = len;
    return 1;
}

void free_file_data(struct file_data *fd) {
    if (fd != NULL && fd->data != NULL) {
        free(fd->data);
        fd->data = NULL;
        fd->size = 0;
        fd->capacity = 0;
    }
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    size_t max_output_size = input->size * 2;
    if (max_output_size < input->size) return 0;
    output->data = malloc(max_output_size);
    if (output->data == NULL) return 0;
    output->capacity = max_output_size;
    output->size = 0;
    size_t i = 0;
    while (i < input->size) {
        if (output->size + 2 > output->capacity) break;
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && count < 255 && input->data[i + count] == current) {
            count++;
        }
        output->data[output->size] = (uint8_t)count;
        output->data[output->size + 1] = current;
        output->size += 2;
        i += count;
    }
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    size_t max_output_size = input->size * 2;
    if (max_output_size < input->size) return 0;
    output->data = malloc(max_output_size);
    if (output->data == NULL) return 0;
    output->capacity = max_output_size;
    output->size = 0;
    for (size_t i = 0; i < input->size; i++) {
        if (output->size >= output->capacity) break;
        output->data[output->size] = input->data[i];
        output->size++;
    }
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    size_t max_output_size = input->size * 2;
    if (max_output_size < input->size) return 0;
    output->data = malloc(max_output_size);
    if (output->data == NULL) return 0;
    output->capacity = max_output_size;
    output->size = 0;
    for (size_t i = 0; i < input->size; i++) {
        if (output->size >= output->capacity) break;
        output->data[output->size] = input->data[i] ^ 0x55;
        output->size++;
    }
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL) return 0;
    int result = 0;
    switch (type) {
        case COMPRESS_NONE:
            if (input->size > 0) {
                output->data = malloc(input->size);
                if (output->data != NULL) {
                    memcpy(output->data, input->data, input->size);
                    output->size = input->size;
                    output->capacity = input->size;
                    result = 1;
                }
            }
            break;
        case COMPRESS_RLE:
            result = compress_rle(input, output);
            break;
        case COMPRESS_LZW:
            result = compress_lzw(input, output);
            break;
        case COMPRESS_HUFFMAN:
            result = compress_huffman(input, output);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

void print_compression_stats(const struct file_data *input, const struct file_data *output) {
    if (input == NULL || output == NULL) return;
    printf("Input size: %zu bytes\n", input->size);
    printf("Output size: %zu bytes\n", output->size);
    if (input->size > 0) {
        double ratio = (double)output->size / (double)input->size * 100.0;
        printf("Compression ratio: %.2f%%\n", ratio);
    }
}

int main(void) {
    char input_buffer[1025];
    printf("Enter