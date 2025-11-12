//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BLOCK_SIZE 4096
#define MAX_DICT_SIZE 256

struct CompressionHeader {
    uint32_t magic;
    uint16_t version;
    uint16_t block_count;
    uint32_t total_size;
};

struct CompressionBlock {
    uint16_t original_size;
    uint16_t compressed_size;
    uint32_t checksum;
    uint8_t data[MAX_BLOCK_SIZE];
};

struct CompressionDict {
    uint8_t symbol;
    uint16_t code;
    uint8_t length;
};

struct CompressionState {
    struct CompressionHeader header;
    struct CompressionDict dictionary[MAX_DICT_SIZE];
    uint16_t dict_size;
    uint32_t current_offset;
};

struct CompressionResult {
    int success;
    size_t input_size;
    size_t output_size;
    uint32_t final_checksum;
};

void init_compression_state(struct CompressionState *state) {
    state->header.magic = 0x434F4D50;
    state->header.version = 1;
    state->header.block_count = 0;
    state->header.total_size = 0;
    state->dict_size = 0;
    state->current_offset = 0;
}

int validate_input(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > MAX_BLOCK_SIZE * 100) return 0;
    return 1;
}

uint32_t calculate_checksum(const uint8_t *data, size_t size) {
    uint32_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum = (checksum << 3) | (checksum >> 29);
        checksum ^= data[i];
    }
    return checksum;
}

int build_dictionary(struct CompressionState *state, const uint8_t *data, size_t size) {
    if (state->dict_size >= MAX_DICT_SIZE) return 0;
    
    uint8_t frequency[256] = {0};
    for (size_t i = 0; i < size; i++) {
        if (data[i] < 255) {
            frequency[data[i]]++;
        }
    }
    
    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0 && state->dict_size < MAX_DICT_SIZE) {
            state->dictionary[state->dict_size].symbol = (uint8_t)i;
            state->dictionary[state->dict_size].code = state->dict_size;
            state->dictionary[state->dict_size].length = 8;
            state->dict_size++;
        }
    }
    return 1;
}

int compress_block(struct CompressionState *state, const uint8_t *input, size_t input_size, struct CompressionBlock *block) {
    if (input_size > MAX_BLOCK_SIZE) return 0;
    
    block->original_size = (uint16_t)input_size;
    block->checksum = calculate_checksum(input, input_size);
    
    size_t output_pos = 0;
    for (size_t i = 0; i < input_size && output_pos < MAX_BLOCK_SIZE; i++) {
        uint8_t symbol = input[i];
        for (uint16_t j = 0; j < state->dict_size; j++) {
            if (state->dictionary[j].symbol == symbol) {
                if (output_pos < MAX_BLOCK_SIZE) {
                    block->data[output_pos++] = (uint8_t)(state->dictionary[j].code & 0xFF);
                }
                break;
            }
        }
    }
    
    block->compressed_size = (uint16_t)output_pos;
    return 1;
}

struct CompressionResult compress_data(const uint8_t *input, size_t input_size) {
    struct CompressionResult result = {0};
    result.input_size = input_size;
    
    if (!validate_input(input, input_size)) {
        return result;
    }
    
    struct CompressionState state;
    init_compression_state(&state);
    
    if (!build_dictionary(&state, input, input_size)) {
        return result;
    }
    
    struct CompressionBlock block;
    if (!compress_block(&state, input, input_size, &block)) {
        return result;
    }
    
    state.header.block_count = 1;
    state.header.total_size = sizeof(struct CompressionHeader) + sizeof(struct CompressionDict) * state.dict_size + sizeof(struct CompressionBlock);
    
    result.success = 1;
    result.output_size = state.header.total_size;
    result.final_checksum = block.checksum;
    
    return result;
}

int main() {
    uint8_t test_data[] = "This is a test string for compression demonstration. It contains various characters and patterns.";
    size_t data_size = sizeof(test_data) - 1;
    
    printf("Input data size: %zu bytes\n", data_size);
    printf("Input data: %s\n", test_data);
    
    struct CompressionResult result = compress_data(test_data, data_size);
    
    if (result.success) {
        printf("Compression successful!\n");
        printf("Output size: %zu bytes\n", result.output_size);
        printf("Compression ratio: %.2f%%\n", (double)result.output_size / data_size * 100);
        printf("Final checksum: 0x%08X\n", result.final_checksum);