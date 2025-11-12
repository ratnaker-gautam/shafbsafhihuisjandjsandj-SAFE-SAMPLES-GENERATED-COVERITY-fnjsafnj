//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    unsigned char *data;
    size_t size;
    size_t capacity;
} ByteBuffer;

void init_buffer(ByteBuffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int resize_buffer(ByteBuffer *buf, size_t new_capacity) {
    if (new_capacity == 0) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
        return 1;
    }
    
    if (new_capacity <= buf->capacity) {
        return 1;
    }
    
    unsigned char *new_data = realloc(buf->data, new_capacity);
    if (new_data == NULL) {
        return 0;
    }
    
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int append_to_buffer(ByteBuffer *buf, unsigned char byte) {
    if (buf->size >= buf->capacity) {
        size_t new_capacity = buf->capacity == 0 ? 16 : buf->capacity * 2;
        if (new_capacity <= buf->capacity || !resize_buffer(buf, new_capacity)) {
            return 0;
        }
    }
    
    buf->data[buf->size] = byte;
    buf->size++;
    return 1;
}

void recursive_compress(const unsigned char *input, size_t pos, size_t length, ByteBuffer *output) {
    if (pos >= length) {
        return;
    }
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    size_t i = pos + 1;
    while (i < length && input[i] == current && count < UCHAR_MAX) {
        count++;
        i++;
    }
    
    if (count > 3 || current == 0xFF) {
        if (!append_to_buffer(output, 0xFF) || !append_to_buffer(output, current) || !append_to_buffer(output, (unsigned char)count)) {
            return;
        }
    } else {
        for (size_t j = 0; j < count; j++) {
            if (!append_to_buffer(output, current)) {
                return;
            }
        }
    }
    
    recursive_compress(input, pos + count, length, output);
}

int compress_data(const unsigned char *input, size_t input_len, ByteBuffer *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    init_buffer(output);
    
    if (input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    recursive_compress(input, 0, input_len, output);
    return output->size > 0;
}

void recursive_decompress(const unsigned char *input, size_t pos, size_t length, ByteBuffer *output) {
    if (pos >= length) {
        return;
    }
    
    if (input[pos] == 0xFF && pos + 2 < length) {
        unsigned char value = input[pos + 1];
        unsigned char count = input[pos + 2];
        
        for (int i = 0; i < count; i++) {
            if (!append_to_buffer(output, value)) {
                return;
            }
        }
        
        recursive_decompress(input, pos + 3, length, output);
    } else {
        if (!append_to_buffer(output, input[pos])) {
            return;
        }
        recursive_decompress(input, pos + 1, length, output);
    }
}

int decompress_data(const unsigned char *input, size_t input_len, ByteBuffer *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    init_buffer(output);
    
    if (input_len > MAX_INPUT_SIZE) {
        return 0;
    }
    
    recursive_decompress(input, 0, input_len, output);
    return output->size > 0;
}

void cleanup_buffer(ByteBuffer *buf) {
    free(buf->data);
    init_buffer(buf);
}

int main() {
    unsigned char test_input[] = "AAAABBBCCCDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPP";
    size_t input_len = sizeof(test_input) - 1;
    
    printf("Original data: %s\n", test_input);
    printf("Original size: %zu bytes\n", input_len);
    
    ByteBuffer compressed;
    if (compress_data(test_input, input_len, &compressed)) {
        printf("Compressed size: %zu bytes\n", compressed.size);
        
        ByteBuffer decompressed;
        if (decompress_data(compressed.data, compressed.size, &decompressed)) {
            printf("Decompressed size: %zu bytes\n", decompressed.size);
            
            if (decompressed.size == input_len && memcmp(test_input, decompressed.data, input_len) == 0) {
                printf("Decompression successful!\n");
            } else {
                printf("Decompression failed!\n");
            }
            
            cleanup_buffer(&decompressed);
        } else {
            printf("Decompression failed!\n");
        }
        
        cleanup_buffer(&compressed);
    } else {
        printf("Compression failed!\n");
    }
    
    return 0