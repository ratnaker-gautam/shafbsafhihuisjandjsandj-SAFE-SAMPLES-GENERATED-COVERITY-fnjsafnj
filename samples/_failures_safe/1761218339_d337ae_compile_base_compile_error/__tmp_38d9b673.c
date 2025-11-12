//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum compression_type {
    COMP_NONE,
    COMP_RLE,
    COMP_LZW,
    COMP_HUFFMAN
};

struct file_data {
    uint8_t *data;
    size_t size;
    size_t capacity;
};

int init_file_data(struct file_data *fd, size_t initial_size) {
    if (fd == NULL || initial_size == 0) return 0;
    fd->data = malloc(initial_size);
    if (fd->data == NULL) return 0;
    fd->size = 0;
    fd->capacity = initial_size;
    return 1;
}

void free_file_data(struct file_data *fd) {
    if (fd != NULL) {
        free(fd->data);
        fd->data = NULL;
        fd->size = 0;
        fd->capacity = 0;
    }
}

int append_data(struct file_data *fd, const uint8_t *data, size_t len) {
    if (fd == NULL || data == NULL || len == 0) return 0;
    if (SIZE_MAX - fd->size < len) return 0;
    if (fd->size + len > fd->capacity) {
        size_t new_cap = fd->capacity * 2;
        if (new_cap < fd->size + len) new_cap = fd->size + len;
        if (new_cap < 16) new_cap = 16;
        uint8_t *new_data = realloc(fd->data, new_cap);
        if (new_data == NULL) return 0;
        fd->data = new_data;
        fd->capacity = new_cap;
    }
    memcpy(fd->data + fd->size, data, len);
    fd->size += len;
    return 1;
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size * 2)) return 0;
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && count < 255 && input->data[i + count] == current) {
            count++;
        }
        uint8_t run[2] = {current, (uint8_t)count};
        if (!append_data(output, run, 2)) {
            free_file_data(output);
            return 0;
        }
        i += count;
    }
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    for (size_t i = 0; i < input->size; i++) {
        uint8_t byte = input->data[i];
        if (!append_data(output, &byte, 1)) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    for (size_t i = 0; i < input->size; i++) {
        uint8_t byte = input->data[i] ^ 0x55;
        if (!append_data(output, &byte, 1)) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    switch (type) {
        case COMP_NONE:
            if (!init_file_data(output, input->size)) return 0;
            return append_data(output, input->data, input->size);
        case COMP_RLE:
            return compress_rle(input, output);
        case COMP_LZW:
            return compress_lzw(input, output);
        case COMP_HUFFMAN:
            return compress_huffman(input, output);
        default:
            return 0;
    }
}

int main(void) {
    struct file_data input, output;
    if (!init_file_data(&input, 64)) {
        fprintf(stderr, "Failed to initialize input buffer\n");
        return 1;
    }
    const uint8_t test_data[] = "AAAABBBCCDAA";
    size_t test_len = strlen((char*)test_data);
    if (!append_data(&input, test_data, test_len)) {
        fprintf(stderr, "Failed to prepare test data\n");
        free_file_data(&input);
        return 1;
    }
    printf("Original data: %s\n", input.data);
    printf("Original size: %zu\n", input.size);
    for (enum compression_type ct = COMP_NONE; ct <= COMP_HUFFMAN; ct++) {
        memset(&output, 0, sizeof(output));
        if (compress_data(ct, &input, &output)) {
            printf("Compression type %d: output size %zu\n", ct, output.size);
            free_file