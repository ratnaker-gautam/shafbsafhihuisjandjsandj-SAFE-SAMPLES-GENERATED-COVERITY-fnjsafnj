//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
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
} Buffer;

void buffer_init(Buffer *buf) {
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int buffer_resize(Buffer *buf, size_t new_capacity) {
    if (new_capacity == 0) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
        return 1;
    }
    
    if (new_capacity < buf->size) {
        return 0;
    }
    
    unsigned char *new_data = realloc(buf->data, new_capacity);
    if (new_data == NULL) {
        return 0;
    }
    
    buf->data = new_data;
    buf->capacity = new_capacity;
    return 1;
}

int buffer_append(Buffer *buf, const unsigned char *data, size_t len) {
    if (buf == NULL || data == NULL || len == 0) {
        return 0;
    }
    
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        if (new_capacity < buf->size + len) {
            new_capacity = buf->size + len;
        }
        if (!buffer_resize(buf, new_capacity)) {
            return 0;
        }
    }
    
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void run_length_encode_recursive(const unsigned char *input, size_t len, size_t pos, Buffer *output) {
    if (pos >= len) {
        return;
    }
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    while (pos + count < len && input[pos + count] == current && count < UCHAR_MAX) {
        count++;
    }
    
    if (count > 3 || current == 0xFF) {
        unsigned char encoded[3] = {0xFF, current, (unsigned char)count};
        buffer_append(output, encoded, 3);
    } else {
        for (size_t i = 0; i < count; i++) {
            buffer_append(output, &current, 1);
        }
    }
    
    run_length_encode_recursive(input, len, pos + count, output);
}

int compress_data(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    buffer_init(output);
    
    if (!buffer_resize(output, input_len * 2)) {
        return 0;
    }
    
    run_length_encode_recursive(input, input_len, 0, output);
    return 1;
}

void print_hex(const unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (len % 16 != 0) {
        printf("\n");
    }
}

int main(void) {
    char input_str[MAX_INPUT_SIZE + 1];
    Buffer input_data;
    Buffer compressed_data;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_str);
    if (input_len > 0 && input_str[input_len - 1] == '\n') {
        input_str[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    buffer_init(&input_data);
    if (!buffer_resize(&input_data, input_len)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    memcpy(input_data.data, input_str, input_len);
    input_data.size = input_len;
    
    if (!compress_data(input_data.data, input_data.size, &compressed_data)) {
        fprintf(stderr, "Compression failed\n");
        buffer_resize(&input_data, 0);
        return 1;
    }
    
    printf("\nOriginal data (%zu bytes):\n", input_data.size);
    print_hex(input_data.data, input_data.size);
    
    printf("\nCompressed data (%zu bytes):\n", compressed_data.size);
    print_hex(compressed_data.data, compressed_data.size);
    
    printf("\nCompression ratio: %.2f%%\n", 
           (double)compressed_data.size / input_data.size * 100);
    
    buffer_resize(&input_data, 0);
    buffer_resize(&compressed_data, 0);
    
    return 0;
}