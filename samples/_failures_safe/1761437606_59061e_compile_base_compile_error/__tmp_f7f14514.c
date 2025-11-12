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
    if (fd != NULL && fd->data != NULL) {
        free(fd->data);
        fd->data = NULL;
        fd->size = 0;
        fd->capacity = 0;
    }
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
            size_t new_cap = output->capacity * 2;
            uint8_t *new_data = realloc(output->data, new_cap);
            if (new_data == NULL) return 0;
            output->data = new_data;
            output->capacity = new_cap;
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
    if (!init_file_data(output, input->size + 256)) return 0;
    
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    
    return 1;
}

int compress_huffman(const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    if (!init_file_data(output, input->size)) return 0;
    
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    
    return 1;
}

int compress_data(enum compression_type type, const struct file_data *input, struct file_data *output) {
    if (input == NULL || output == NULL || input->size == 0) return 0;
    
    switch (type) {
        case COMP_NONE:
            if (!init_file_data(output, input->size)) return 0;
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
    char buffer[256];
    enum compression_type comp_type;
    
    if (!init_file_data(&input, 1024)) {
        printf("Failed to initialize input buffer\n");
        return 1;
    }
    
    printf("Enter data to compress: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Failed to read input\n");
        free_file_data(&input);
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len > input.capacity) {
        printf("Input too large\n");
        free_file_data(&input);
        return 1;
    }
    
    memcpy(input.data, buffer, len);
    input.size = len;
    
    printf("Select compression type:\n");
    printf("0: None\n1: RLE\n2: LZW\n3: Huffman\n");
    printf("Enter choice (0-3): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Failed to read choice\n");
        free_file_data(&input);
        return 1;
    }
    
    int choice = atoi(buffer);
    if (choice < 0 || choice > 3) {
        printf("Invalid choice\n");
        free_file_data(&input);
        return 1;
    }
    
    comp_type = (enum compression_type)choice;
    
    if (!compress_data(comp_type, &input, &output)) {
        printf("Compression failed\n");
        free_file_data(&input);
        return 1;
    }
    
    printf("Original