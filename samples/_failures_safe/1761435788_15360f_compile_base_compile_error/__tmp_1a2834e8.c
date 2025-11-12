//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 2048

typedef struct {
    uint8_t *data;
    size_t size;
} Buffer;

typedef size_t (*CompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);
typedef size_t (*DecompressFunc)(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size);

size_t run_length_encode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0) {
        return 0;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size && out_index + 2 < output_size) {
        uint8_t current = input[i];
        size_t count = 1;
        
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }
        
        output[out_index++] = current;
        output[out_index++] = (uint8_t)count;
        i += count;
    }
    
    return out_index;
}

size_t run_length_decode(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size == 0 || input_size % 2 != 0) {
        return 0;
    }
    
    size_t out_index = 0;
    size_t i = 0;
    
    while (i < input_size && out_index < output_size) {
        uint8_t value = input[i++];
        uint8_t count = input[i++];
        
        for (uint8_t j = 0; j < count && out_index < output_size; j++) {
            output[out_index++] = value;
        }
    }
    
    return out_index;
}

size_t null_compress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    if (input == NULL || output == NULL || input_size == 0 || output_size < input_size) {
        return 0;
    }
    
    memcpy(output, input, input_size);
    return input_size;
}

size_t null_decompress(const uint8_t *input, size_t input_size, uint8_t *output, size_t output_size) {
    return null_compress(input, input_size, output, output_size);
}

int compress_data(CompressFunc compress, const Buffer *input, Buffer *output) {
    if (compress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) {
        return 0;
    }
    
    output->size = compress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

int decompress_data(DecompressFunc decompress, const Buffer *input, Buffer *output) {
    if (decompress == NULL || input == NULL || output == NULL || input->data == NULL) {
        return 0;
    }
    
    if (input->size > MAX_INPUT_SIZE) {
        return 0;
    }
    
    output->data = malloc(MAX_OUTPUT_SIZE);
    if (output->data == NULL) {
        return 0;
    }
    
    output->size = decompress(input->data, input->size, output->data, MAX_OUTPUT_SIZE);
    if (output->size == 0) {
        free(output->data);
        output->data = NULL;
        return 0;
    }
    
    return 1;
}

void cleanup_buffer(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        buf->data = NULL;
        buf->size = 0;
    }
}

int main() {
    CompressFunc compressors[] = {run_length_encode, null_compress};
    DecompressFunc decompressors[] = {run_length_decode, null_decompress};
    const char *names[] = {"Run-length encoding", "Null compression"};
    int num_methods = 2;
    
    printf("Enter text to compress (max %d characters): ", MAX_INPUT_SIZE);
    
    char input_text[MAX_INPUT_SIZE + 1];
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[--input_len] = '\0';
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    Buffer original = {(uint8_t *)input_text, input_len};
    Buffer compressed = {NULL, 0};
    Buffer decompressed = {NULL, 0};