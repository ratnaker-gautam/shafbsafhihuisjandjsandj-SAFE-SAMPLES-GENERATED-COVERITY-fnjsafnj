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
        if (new_capacity < buf->size || new_capacity < len) {
            return 0;
        }
        if (!buffer_resize(buf, new_capacity)) {
            return 0;
        }
    }
    
    memcpy(buf->data + buf->size, data, len);
    buf->size += len;
    return 1;
}

void buffer_cleanup(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
        buf->capacity = 0;
    }
}

size_t compress_run_length_recursive(const unsigned char *input, size_t input_len, size_t pos, Buffer *output) {
    if (pos >= input_len) {
        return pos;
    }
    
    unsigned char current = input[pos];
    size_t count = 1;
    
    while (pos + count < input_len && input[pos + count] == current && count < 255) {
        count++;
    }
    
    if (count >= 3 || current == 0xFF) {
        unsigned char encoded[2] = {0xFF, (unsigned char)count};
        if (!buffer_append(output, encoded, 2)) {
            return input_len;
        }
        if (!buffer_append(output, &current, 1)) {
            return input_len;
        }
    } else {
        if (!buffer_append(output, &current, 1)) {
            return input_len;
        }
        for (size_t i = 1; i < count; i++) {
            if (!buffer_append(output, &current, 1)) {
                return input_len;
            }
        }
    }
    
    return compress_run_length_recursive(input, input_len, pos + count, output);
}

int compress_data(const unsigned char *input, size_t input_len, Buffer *output) {
    if (input == NULL || output == NULL || input_len == 0) {
        return 0;
    }
    
    buffer_init(output);
    
    if (input_len > SIZE_MAX / 2) {
        return 0;
    }
    
    if (!buffer_resize(output, input_len * 2)) {
        return 0;
    }
    
    size_t final_pos = compress_run_length_recursive(input, input_len, 0, output);
    
    return (final_pos == input_len) ? 1 : 0;
}

void print_buffer_hex(const Buffer *buf) {
    if (buf == NULL || buf->data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < buf->size; i++) {
        printf("%02X", buf->data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 8 == 0) {
            printf("  ");
        } else {
            printf(" ");
        }
    }
    if (buf->size % 16 != 0) {
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    buffer_init(&input_data);
    if (!buffer_resize(&input_data, input_len)) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    memcpy(input_data.data, input_str, input_len);