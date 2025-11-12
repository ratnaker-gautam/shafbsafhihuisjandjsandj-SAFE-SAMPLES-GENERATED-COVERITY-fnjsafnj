//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 16;
    buf->data = malloc(initial_capacity);
    if (!buf->data) exit(EXIT_FAILURE);
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            if (new_capacity > SIZE_MAX / 2) exit(EXIT_FAILURE);
            new_capacity *= 2;
        }
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (!new_data) exit(EXIT_FAILURE);
        buf->data = new_data;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->size, src, len);
    buf->size += len;
}

void buffer_free(Buffer *buf) {
    free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->capacity = 0;
}

int compress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || input_len == 0 || !output) return -1;
    if (input_len > MAX_INPUT_SIZE) return -1;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end) {
        uint8_t value = *current;
        size_t count = 1;
        
        const uint8_t *next = current + 1;
        while (next < end && *next == value && count < 255) {
            count++;
            next++;
        }
        
        uint8_t run[2] = {value, (uint8_t)count};
        buffer_append(output, run, 2);
        
        current = next;
    }
    
    return 0;
}

int decompress_rle(const uint8_t *input, size_t input_len, Buffer *output) {
    if (!input || input_len == 0 || !output) return -1;
    if (input_len % 2 != 0) return -1;
    if (input_len > MAX_OUTPUT_SIZE) return -1;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        
        if (output->size + count > MAX_OUTPUT_SIZE) return -1;
        
        for (uint8_t i = 0; i < count; i++) {
            buffer_append(output, &value, 1);
        }
        
        current += 2;
    }
    
    return 0;
}

void print_buffer_hex(const Buffer *buf) {
    if (!buf || buf->size == 0) return;
    
    const uint8_t *ptr = buf->data;
    const uint8_t *end = buf->data + buf->size;
    
    while (ptr < end) {
        printf("%02x", *ptr);
        ptr++;
    }
    printf("\n");
}

int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int main(void) {
    Buffer input_buf, compressed_buf, decompressed_buf;
    
    buffer_init(&input_buf, 64);
    buffer_init(&compressed_buf, 64);
    buffer_init(&decompressed_buf, 64);
    
    printf("Enter data to compress (max %d bytes as hex digits): ", MAX_INPUT_SIZE);
    
    char hex_input[MAX_INPUT_SIZE * 2 + 1];
    if (!fgets(hex_input, sizeof(hex_input), stdin)) {
        printf("Input error\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    size_t hex_len = strlen(hex_input);
    if (hex_len > 0 && hex_input[hex_len - 1] == '\n') {
        hex_input[hex_len - 1] = '\0';
        hex_len--;
    }
    
    if (hex_len % 2 != 0) {
        printf("Invalid hex input: odd number of digits\n");
        buffer_free(&input_buf);
        buffer_free(&compressed_buf);
        buffer_free(&decompressed_buf);
        return EXIT_FAILURE;
    }
    
    size_t data_len = hex_len / 2;
    if