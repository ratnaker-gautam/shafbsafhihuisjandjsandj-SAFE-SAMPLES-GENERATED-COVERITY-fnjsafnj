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

int init_file_data(struct file_data *fd, size_t initial_capacity) {
    if (fd == NULL || initial_capacity == 0) return 0;
    fd->data = malloc(initial_capacity);
    if (fd->data == NULL) return 0;
    fd->size = 0;
    fd->capacity = initial_capacity;
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

int resize_file_data(struct file_data *fd, size_t new_capacity) {
    if (fd == NULL || new_capacity == 0) return 0;
    uint8_t *new_data = realloc(fd->data, new_capacity);
    if (new_data == NULL) return 0;
    fd->data = new_data;
    fd->capacity = new_capacity;
    if (fd->size > new_capacity) fd->size = new_capacity;
    return 1;
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    size_t needed = input->size * 2 + 2;
    if (output->capacity < needed && !resize_file_data(output, needed)) return 0;
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; ) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && input->data[i + count] == current && count < 255) count++;
        if (out_idx + 2 > output->capacity) return 0;
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    output->size = out_idx;
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    size_t needed = input->size + 32;
    if (output->capacity < needed && !resize_file_data(output, needed)) return 0;
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    size_t needed = input->size + 16;
    if (output->capacity < needed && !resize_file_data(output, needed)) return 0;
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL) return 0;
    switch (type) {
        case COMP_NONE:
            if (output->capacity < input->size && !resize_file_data(output, input->size)) return 0;
            memcpy(output->data, input->data, input->size);
            output->size = input->size;
            return 1;
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
    if (!init_file_data(&input, 1024) || !init_file_data(&output, 2048)) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter data to compress (max 1023 bytes): ");
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        free_file_data(&input);
        free_file_data(&output);
        return 1;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[--len] = '\0';
    if (len == 0) {
        printf("No input data\n");
        free_file_data(&input);
        free_file_data(&output);
        return 1;
    }

    if (input.capacity < len && !resize_file_data(&input, len)) {
        printf("Memory allocation failed\n");
        free_file_data(&input);
        free_file_data(&output);
        return 1;
    }
    memcpy(input.data, buffer, len);
    input.size = len;

    printf("Select compression type:\n");
    printf("0: None\n1: RLE\n2: LZW\n3: Huffman\n");
    int choice;
    if