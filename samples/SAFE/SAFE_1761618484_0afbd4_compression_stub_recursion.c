//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

typedef struct {
    uint8_t data[MAX_INPUT_SIZE];
    size_t size;
} Buffer;

Buffer compress_chunk(const uint8_t* data, size_t start, size_t end) {
    Buffer result = {0};
    
    if (start >= end) {
        return result;
    }
    
    size_t count = 1;
    size_t current = start;
    
    while (current + 1 < end && data[current] == data[current + 1]) {
        count++;
        current++;
        
        if (count == 255) {
            break;
        }
    }
    
    if (count > 1) {
        result.data[0] = data[start];
        result.data[1] = (uint8_t)count;
        result.size = 2;
    } else {
        size_t literal_start = start;
        size_t literal_end = start;
        
        while (literal_end + 1 < end && data[literal_end] != data[literal_end + 1] && 
               (literal_end - literal_start) < 127) {
            literal_end++;
        }
        
        size_t literal_len = literal_end - literal_start + 1;
        result.data[0] = 0x80 | (uint8_t)literal_len;
        
        for (size_t i = 0; i < literal_len; i++) {
            if (i + 1 < MAX_INPUT_SIZE) {
                result.data[i + 1] = data[literal_start + i];
            }
        }
        
        result.size = literal_len + 1;
    }
    
    return result;
}

void compress_recursive(const uint8_t* data, size_t start, size_t end, Buffer* output) {
    if (start >= end || output->size >= MAX_INPUT_SIZE - 10) {
        return;
    }
    
    Buffer chunk = compress_chunk(data, start, end);
    
    if (chunk.size > 0 && output->size + chunk.size < MAX_INPUT_SIZE) {
        memcpy(output->data + output->size, chunk.data, chunk.size);
        output->size += chunk.size;
        
        size_t next_start = start;
        if (chunk.data[0] & 0x80) {
            next_start += (chunk.data[0] & 0x7F);
        } else {
            next_start += chunk.data[1];
        }
        
        compress_recursive(data, next_start, end, output);
    }
}

int main() {
    uint8_t input[MAX_INPUT_SIZE];
    Buffer output = {0};
    
    printf("Enter data to compress (max %d bytes): ", MAX_INPUT_SIZE);
    
    size_t bytes_read = 0;
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n' && bytes_read < MAX_INPUT_SIZE - 1) {
        input[bytes_read++] = (uint8_t)ch;
    }
    
    if (bytes_read == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (bytes_read >= MAX_INPUT_SIZE) {
        printf("Input too large.\n");
        return 1;
    }
    
    compress_recursive(input, 0, bytes_read, &output);
    
    printf("Original size: %zu bytes\n", bytes_read);
    printf("Compressed size: %zu bytes\n", output.size);
    printf("Compression ratio: %.2f%%\n", 
           (bytes_read > 0) ? (100.0 * output.size / bytes_read) : 0.0);
    
    printf("Compressed data: ");
    for (size_t i = 0; i < output.size; i++) {
        printf("%02X ", output.data[i]);
    }
    printf("\n");
    
    return 0;
}