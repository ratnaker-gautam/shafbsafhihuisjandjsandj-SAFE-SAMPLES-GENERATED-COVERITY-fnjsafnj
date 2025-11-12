//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_init(Buffer *buf, size_t initial_capacity) {
    if (initial_capacity == 0) initial_capacity = 1;
    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) abort();
    buf->size = 0;
    buf->capacity = initial_capacity;
}

void buffer_append(Buffer *buf, const uint8_t *src, size_t len) {
    if (len == 0) return;
    if (buf->size + len > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) new_capacity *= 2;
        uint8_t *new_data = realloc(buf->data, new_capacity);
        if (new_data == NULL) abort();
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
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0) return 0;
    
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
    if (input == NULL || output == NULL) return -1;
    if (input_len == 0) return 0;
    if (input_len % 2 != 0) return -1;
    
    const uint8_t *current = input;
    const uint8_t *end = input + input_len;
    
    while (current < end) {
        uint8_t value = *current;
        uint8_t count = *(current + 1);
        
        if (count == 0) return -1;
        
        for (uint8_t i = 0; i < count; i++) {
            buffer_append(output, &value, 1);
        }
        
        current += 2;
    }
    
    return 0;
}

int main(void) {
    uint8_t input[MAX_INPUT_SIZE];
    size_t input_len;
    
    printf("Enter input size (max %d): ", MAX_INPUT_SIZE);
    if (scanf("%zu", &input_len) != 1) {
        printf("Invalid input size\n");
        return 1;
    }
    
    if (input_len == 0 || input_len > MAX_INPUT_SIZE) {
        printf("Invalid input size\n");
        return 1;
    }
    
    printf("Enter %zu bytes (0-255): ", input_len);
    for (size_t i = 0; i < input_len; i++) {
        unsigned int temp;
        if (scanf("%u", &temp) != 1) {
            printf("Invalid input byte\n");
            return 1;
        }
        if (temp > 255) {
            printf("Byte value out of range\n");
            return 1;
        }
        input[i] = (uint8_t)temp;
    }
    
    Buffer compressed;
    buffer_init(&compressed, 64);
    
    if (compress_rle(input, input_len, &compressed) != 0) {
        printf("Compression failed\n");
        buffer_free(&compressed);
        return 1;
    }
    
    printf("Compressed data (%zu bytes): ", compressed.size);
    for (size_t i = 0; i < compressed.size; i++) {
        printf("%u ", compressed.data[i]);
    }
    printf("\n");
    
    Buffer decompressed;
    buffer_init(&decompressed, 64);
    
    if (decompress_rle(compressed.data, compressed.size, &decompressed) != 0) {
        printf("Decompression failed\n");
        buffer_free(&compressed);
        buffer_free(&decompressed);
        return 1;
    }
    
    printf("Decompressed data (%zu bytes): ", decompressed.size);
    for (size_t i = 0; i < decompressed.size; i++) {
        printf("%u ", decompressed.data[i]);
    }
    printf("\n");
    
    if (decompressed.size == input_len && memcmp(decompressed.data, input, input_len) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    buffer_free(&compressed);
    buffer