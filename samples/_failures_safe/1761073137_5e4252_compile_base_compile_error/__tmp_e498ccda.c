//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

int init_file_data(struct file_data *fd, size_t initial_size) {
    if (fd == NULL || initial_size == 0) return 0;
    fd->data = malloc(initial_size);
    if (fd->data == NULL) return 0;
    fd->size = 0;
    fd->capacity = initial_size;
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

int append_byte(struct file_data *fd, uint8_t byte) {
    if (fd == NULL) return 0;
    if (fd->size >= fd->capacity) {
        size_t new_capacity = fd->capacity * 2;
        if (new_capacity <= fd->capacity) return 0;
        uint8_t *new_data = realloc(fd->data, new_capacity);
        if (new_data == NULL) return 0;
        fd->data = new_data;
        fd->capacity = new_capacity;
    }
    fd->data[fd->size] = byte;
    fd->size++;
    return 1;
}

int compress_rle(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size * 2)) return 0;
    
    size_t i = 0;
    while (i < input->size) {
        uint8_t current = input->data[i];
        size_t count = 1;
        while (i + count < input->size && input->data[i + count] == current && count < 255) {
            count++;
        }
        if (!append_byte(output, (uint8_t)count) || !append_byte(output, current)) {
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
        if (!append_byte(output, input->data[i])) {
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
        if (!append_byte(output, input->data[i] ^ 0x55)) {
            free_file_data(output);
            return 0;
        }
    }
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    switch (type) {
        case COMPRESS_NONE:
            if (!init_file_data(output, input->size)) return 0;
            for (size_t i = 0; i < input->size; i++) {
                if (!append_byte(output, input->data[i])) {
                    free_file_data(output);
                    return 0;
                }
            }
            return 1;
            
        case COMPRESS_RLE:
            return compress_rle(input, output);
            
        case COMPRESS_LZW:
            return compress_lzw(input, output);
            
        case COMPRESS_HUFFMAN:
            return compress_huffman(input, output);
            
        default:
            return 0;
    }
}

int get_user_choice(void) {
    char buffer[16];
    int choice;
    
    printf("Select compression method:\n");
    printf("1: None\n");
    printf("2: RLE\n");
    printf("3: LZW\n");
    printf("4: Huffman\n");
    printf("Choice: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    if (sscanf(buffer, "%d", &choice) != 1) return -1;
    if (choice < 1 || choice > 4) return -1;
    
    return choice - 1;
}

int get_input_data(struct file_data *input) {
    char buffer[256];
    printf("Enter text to compress (max 255 chars): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if