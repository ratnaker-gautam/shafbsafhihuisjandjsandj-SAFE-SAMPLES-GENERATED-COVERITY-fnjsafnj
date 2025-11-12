//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_BLOCK_SIZE 4096
#define MAX_DICT_SIZE 256

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t dict_entries;
    uint8_t block_size;
};

struct DictionaryEntry {
    uint8_t symbol;
    uint16_t code;
    uint8_t code_length;
};

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    uint16_t size;
    uint16_t compressed_bits;
};

struct CompressionState {
    struct CompressionHeader header;
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    struct CompressionBlock current_block;
    uint32_t total_input_bytes;
    uint32_t total_output_bits;
    uint8_t bit_buffer;
    uint8_t bit_count;
};

void init_compression_state(struct CompressionState *state) {
    if (state == NULL) return;
    memset(state, 0, sizeof(*state));
    state->header.magic = 0x43535455;
    state->current_block.size = 0;
    state->bit_buffer = 0;
    state->bit_count = 0;
}

int validate_input_data(const uint8_t *data, size_t size) {
    if (data == NULL) return 0;
    if (size == 0) return 0;
    if (size > UINT32_MAX) return 0;
    return 1;
}

int build_dictionary(struct CompressionState *state, const uint8_t *data, size_t size) {
    if (state == NULL || data == NULL || size == 0) return 0;
    
    uint8_t freq[256] = {0};
    size_t i;
    
    for (i = 0; i < size && i < UINT32_MAX; i++) {
        freq[data[i]]++;
    }
    
    uint16_t entries = 0;
    for (i = 0; i < 256; i++) {
        if (freq[i] > 0 && entries < MAX_DICT_SIZE) {
            state->dictionary[entries].symbol = (uint8_t)i;
            state->dictionary[entries].code = entries;
            state->dictionary[entries].code_length = 8;
            entries++;
        }
    }
    
    state->header.dict_entries = entries;
    return 1;
}

int compress_block(struct CompressionState *state, const uint8_t *data, size_t size) {
    if (state == NULL || data == NULL || size == 0) return 0;
    if (size > MAX_BLOCK_SIZE) return 0;
    
    state->current_block.size = (uint16_t)size;
    memcpy(state->current_block.data, data, size);
    
    state->current_block.compressed_bits = 0;
    state->bit_buffer = 0;
    state->bit_count = 0;
    
    size_t i;
    for (i = 0; i < size; i++) {
        uint8_t symbol = data[i];
        uint16_t code = 0;
        uint8_t code_length = 8;
        
        uint16_t j;
        for (j = 0; j < state->header.dict_entries; j++) {
            if (state->dictionary[j].symbol == symbol) {
                code = state->dictionary[j].code;
                code_length = state->dictionary[j].code_length;
                break;
            }
        }
        
        state->current_block.compressed_bits += code_length;
        state->total_output_bits += code_length;
    }
    
    state->total_input_bytes += (uint32_t)size;
    return 1;
}

void finalize_compression(struct CompressionState *state) {
    if (state == NULL) return;
    
    state->header.original_size = state->total_input_bytes;
    state->header.compressed_size = (state->total_output_bits + 7) / 8;
    state->header.block_size = 64;
}

int write_compressed_output(const struct CompressionState *state, FILE *output) {
    if (state == NULL || output == NULL) return 0;
    
    if (fwrite(&state->header, sizeof(state->header), 1, output) != 1) return 0;
    if (fwrite(state->dictionary, sizeof(struct DictionaryEntry), state->header.dict_entries, output) != state->header.dict_entries) return 0;
    
    uint32_t dummy_data = 0xDEADBEEF;
    if (fwrite(&dummy_data, sizeof(dummy_data), 1, output) != 1) return 0;
    
    return 1;
}

int main(void) {
    struct CompressionState state;
    init_compression_state(&state);
    
    uint8_t input_data[] = {
        'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!',
        'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 't', 'e',
        's', 't', ' ', 's', 't', 'r', 'i', 'n', 'g', '.'
    };
    
    size_t input_size = sizeof(input_data);
    
    if (!validate_input_data(input_data, input_size)) {
        fprintf