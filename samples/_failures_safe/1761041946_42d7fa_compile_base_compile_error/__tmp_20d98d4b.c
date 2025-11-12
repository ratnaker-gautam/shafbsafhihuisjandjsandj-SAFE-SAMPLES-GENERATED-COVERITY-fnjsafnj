//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BLOCK_SIZE 1024
#define MAX_DICT_SIZE 256

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    uint32_t checksum;
};

struct DictionaryEntry {
    uint8_t pattern[16];
    uint8_t replacement;
    size_t frequency;
};

struct CompressionContext {
    struct CompressionBlock current_block;
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
    size_t total_compressed;
    size_t total_original;
};

void initialize_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->dict_size = 0;
    ctx->total_compressed = 0;
    ctx->total_original = 0;
}

uint32_t calculate_checksum(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return 0;
    uint32_t checksum = 0;
    for (size_t i = 0; i < size; i++) {
        checksum = (checksum * 31) + data[i];
    }
    return checksum;
}

int validate_block(const struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (block->size > MAX_BLOCK_SIZE) return 0;
    if (block->size == 0) return 0;
    return 1;
}

int add_dictionary_entry(struct CompressionContext *ctx, const uint8_t *pattern, size_t pattern_size, uint8_t replacement) {
    if (ctx == NULL || pattern == NULL || pattern_size == 0 || pattern_size > 16) return 0;
    if (ctx->dict_size >= MAX_DICT_SIZE) return 0;
    
    struct DictionaryEntry *entry = &ctx->dictionary[ctx->dict_size];
    memcpy(entry->pattern, pattern, pattern_size);
    if (pattern_size < 16) {
        memset(entry->pattern + pattern_size, 0, 16 - pattern_size);
    }
    entry->replacement = replacement;
    entry->frequency = 0;
    ctx->dict_size++;
    return 1;
}

size_t compress_block(struct CompressionContext *ctx, const struct CompressionBlock *input, struct CompressionBlock *output) {
    if (!validate_block(input) || output == NULL || ctx == NULL) return 0;
    
    memcpy(output->data, input->data, input->size);
    output->size = input->size;
    output->checksum = calculate_checksum(output->data, output->size);
    
    size_t compressed_size = output->size;
    for (size_t i = 0; i < ctx->dict_size && i < input->size; i++) {
        for (size_t j = 0; j < input->size - 1; j++) {
            if (input->data[j] == ctx->dictionary[i].pattern[0] && 
                input->data[j + 1] == ctx->dictionary[i].pattern[1]) {
                if (compressed_size > 0) compressed_size--;
                ctx->dictionary[i].frequency++;
            }
        }
    }
    
    return compressed_size;
}

void process_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    struct CompressionBlock compressed;
    size_t new_size = compress_block(ctx, &ctx->current_block, &compressed);
    
    if (new_size > 0) {
        ctx->total_compressed += new_size;
        ctx->total_original += ctx->current_block.size;
        
        double ratio = (ctx->current_block.size > 0) ? 
                      (double)new_size / ctx->current_block.size : 0.0;
        
        printf("Block compressed: %zu -> %zu bytes (ratio: %.2f)\n", 
               ctx->current_block.size, new_size, ratio);
    }
}

int read_input_block(struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    
    printf("Enter block size (1-%d): ", MAX_BLOCK_SIZE);
    int size;
    if (scanf("%d", &size) != 1) return 0;
    if (size <= 0 || size > MAX_BLOCK_SIZE) return 0;
    
    printf("Enter %d bytes as space-separated values (0-255): ", size);
    for (int i = 0; i < size; i++) {
        int value;
        if (scanf("%d", &value) != 1) return 0;
        if (value < 0 || value > 255) return 0;
        ctx->current_block.data[i] = (uint8_t)value;
    }
    
    ctx->current_block.size = size;
    ctx->current_block.checksum = calculate_checksum(ctx->current_block.data, size);
    return 1;
}

void initialize_default_dictionary(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    uint8_t common1[] = {0x20, 0x20};
    uint8_t common2[] = {0x74, 0x68};
    uint8_t common3[] = {0x65, 0x6E};
    
    add_dictionary_entry(ctx, common1, 2, 0x81);
    add_dictionary_entry(ctx, common2, 2