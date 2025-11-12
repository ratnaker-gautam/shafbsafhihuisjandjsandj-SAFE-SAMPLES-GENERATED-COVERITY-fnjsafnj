//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    uint8_t* data;
    size_t size;
} Buffer;

typedef Buffer (*CompressFunc)(const Buffer* input);
typedef Buffer (*DecompressFunc)(const Buffer* input);

Buffer null_compress(const Buffer* input) {
    Buffer output;
    if (input == NULL || input->data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input->size;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input->data, output.size);
    return output;
}

Buffer null_decompress(const Buffer* input) {
    Buffer output;
    if (input == NULL || input->data == NULL) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.size = input->size;
    output.data = malloc(output.size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    memcpy(output.data, input->data, output.size);
    return output;
}

Buffer run_length_encode(const Buffer* input) {
    Buffer output;
    if (input == NULL || input->data == NULL || input->size == 0) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    size_t max_size = input->size * 2;
    if (max_size < input->size) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    output.data = malloc(max_size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input->size) {
        uint8_t current = input->data[in_pos];
        size_t count = 1;
        
        while (in_pos + count < input->size && input->data[in_pos + count] == current && count < 255) {
            count++;
        }
        
        if (out_pos + 2 > max_size) {
            free(output.data);
            output.data = NULL;
            output.size = 0;
            return output;
        }
        
        output.data[out_pos++] = (uint8_t)count;
        output.data[out_pos++] = current;
        in_pos += count;
    }
    
    output.size = out_pos;
    return output;
}

Buffer run_length_decode(const Buffer* input) {
    Buffer output;
    if (input == NULL || input->data == NULL || input->size == 0 || input->size % 2 != 0) {
        output.data = NULL;
        output.size = 0;
        return output;
    }
    
    size_t estimated_size = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        if (estimated_size + input->data[i] < estimated_size) {
            output.data = NULL;
            output.size = 0;
            return output;
        }
        estimated_size += input->data[i];
    }
    
    output.data = malloc(estimated_size);
    if (output.data == NULL) {
        output.size = 0;
        return output;
    }
    
    size_t out_pos = 0;
    for (size_t i = 0; i < input->size; i += 2) {
        uint8_t count = input->data[i];
        uint8_t value = input->data[i + 1];
        
        if (out_pos + count > estimated_size || out_pos + count < out_pos) {
            free(output.data);
            output.data = NULL;
            output.size = 0;
            return output;
        }
        
        for (uint8_t j = 0; j < count; j++) {
            output.data[out_pos++] = value;
        }
    }
    
    output.size = out_pos;
    return output;
}

void process_compression(CompressFunc compress, DecompressFunc decompress, const char* name) {
    printf("Testing %s compression\n", name);
    
    uint8_t test_data[] = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    Buffer input;
    input.data = test_data;
    input.size = sizeof(test_data);
    
    Buffer compressed = compress(&input);
    if (compressed.data == NULL || compressed.size == 0) {
        printf("Compression failed\n");
        return;
    }
    
    printf("Original size: %zu, Compressed size: %zu\n", input.size, compressed.size);
    
    Buffer decompressed = decompress(&compressed);
    if (decompressed.data == NULL || decompressed.size == 0) {
        printf("Decompression failed\n");
        free(compressed.data);
        return;
    }
    
    if (decompressed.size == input.size && memcmp(decompressed.data, input.data, input.size) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    free(compressed.data);
    free(decompressed.data);
}

int main() {
    CompressFunc compressors[] = {null_compress, run_length_encode};
    DecompressFunc