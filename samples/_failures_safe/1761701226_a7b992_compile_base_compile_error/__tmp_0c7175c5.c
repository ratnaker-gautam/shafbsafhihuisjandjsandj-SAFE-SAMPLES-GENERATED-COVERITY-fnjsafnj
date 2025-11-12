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
    if (fd != NULL) {
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
    if (!init_file_data(output, input->size * 2)) return 0;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < input->size; ) {
        uint8_t current = input->data[i];
        size_t count = 1;
        
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        
        if (out_idx + 2 > output->capacity) {
            if (!resize_file_data(output, output->capacity * 2)) {
                free_file_data(output);
                return 0;
            }
        }
        
        output->data[out_idx++] = (uint8_t)count;
        output->data[out_idx++] = current;
        i += count;
    }
    
    output->size = out_idx;
    return 1;
}

int compress_lzw(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size * 2)) return 0;
    
    for (size_t i = 0; i < input->size && i < output->capacity; i++) {
        output->data[i] = input->data[i] ^ 0xAA;
    }
    output->size = input->size;
    
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size + 8)) return 0;
    
    uint32_t checksum = 0;
    for (size_t i = 0; i < input->size; i++) {
        checksum += input->data[i];
    }
    
    memcpy(output->data, input->data, input->size);
    memcpy(output->data + input->size, &checksum, sizeof(checksum));
    output->size = input->size + sizeof(checksum);
    
    return 1;
}

int compress_none(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    return 1;
}

int main(void) {
    struct file_data input, output;
    enum compression_type comp_type;
    int choice;
    
    printf("Enter compression type (0=None, 1=RLE, 2=LZW, 3=Huffman): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 3) {
        printf("Invalid compression type\n");
        return 1;
    }
    comp_type = (enum compression_type)choice;
    
    printf("Enter data size (1-1000): ");
    size_t data_size;
    if (scanf("%zu", &data_size) != 1) {
        printf("Invalid size\n");
        return 1;
    }
    
    if (data_size < 1 || data_size > 1000) {
        printf("Size out of range\n");
        return 1;
    }
    
    if (!init_file_data(&input, data_size)) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter %zu bytes of data (0-255): ", data_size);
    for (size_t i = 0; i < data_size; i++) {
        int value;
        if (scanf("%d", &value) !=