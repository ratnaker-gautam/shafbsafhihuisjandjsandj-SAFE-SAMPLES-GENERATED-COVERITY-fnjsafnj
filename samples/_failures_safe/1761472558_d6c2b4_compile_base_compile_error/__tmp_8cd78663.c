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
    uint8_t algorithm;
    uint8_t checksum;
};

struct DictionaryEntry {
    uint8_t symbol;
    uint32_t frequency;
    uint8_t code_length;
    uint16_t code;
};

struct CompressionBlock {
    uint8_t data[MAX_BLOCK_SIZE];
    size_t size;
    struct DictionaryEntry dictionary[MAX_DICT_SIZE];
    size_t dict_size;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock current_block;
    uint32_t total_processed;
    uint8_t state;
};

void init_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535442;
    ctx->header.algorithm = 1;
    ctx->state = 0;
}

int validate_block_size(size_t size) {
    return (size > 0 && size <= MAX_BLOCK_SIZE);
}

int build_frequency_dict(const uint8_t *data, size_t size, struct DictionaryEntry *dict, size_t *dict_size) {
    if (data == NULL || dict == NULL || dict_size == NULL) return 0;
    if (size == 0 || size > MAX_BLOCK_SIZE) return 0;
    
    uint32_t frequencies[256] = {0};
    
    for (size_t i = 0; i < size; i++) {
        frequencies[data[i]]++;
    }
    
    *dict_size = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            if (*dict_size >= MAX_DICT_SIZE) return 0;
            dict[*dict_size].symbol = (uint8_t)i;
            dict[*dict_size].frequency = frequencies[i];
            dict[*dict_size].code_length = 0;
            dict[*dict_size].code = 0;
            (*dict_size)++;
        }
    }
    
    return 1;
}

int compress_block(struct CompressionBlock *block) {
    if (block == NULL) return 0;
    if (!validate_block_size(block->size)) return 0;
    
    if (!build_frequency_dict(block->data, block->size, block->dictionary, &block->dict_size)) {
        return 0;
    }
    
    for (size_t i = 0; i < block->dict_size - 1; i++) {
        for (size_t j = 0; j < block->dict_size - i - 1; j++) {
            if (block->dictionary[j].frequency < block->dictionary[j + 1].frequency) {
                struct DictionaryEntry temp = block->dictionary[j];
                block->dictionary[j] = block->dictionary[j + 1];
                block->dictionary[j + 1] = temp;
            }
        }
    }
    
    for (size_t i = 0; i < block->dict_size; i++) {
        if (i < 8) {
            block->dictionary[i].code_length = 3;
            block->dictionary[i].code = (uint16_t)i;
        } else if (i < 16) {
            block->dictionary[i].code_length = 4;
            block->dictionary[i].code = (uint16_t)(0x80 | i);
        } else {
            block->dictionary[i].code_length = 8;
            block->dictionary[i].code = (uint16_t)block->dictionary[i].symbol;
        }
    }
    
    return 1;
}

int process_input_data(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL) return 0;
    if (size == 0 || size > MAX_BLOCK_SIZE) return 0;
    
    memcpy(ctx->current_block.data, data, size);
    ctx->current_block.size = size;
    
    if (!compress_block(&ctx->current_block)) {
        return 0;
    }
    
    ctx->header.original_size += (uint32_t)size;
    ctx->header.compressed_size += (uint32_t)(size / 2);
    ctx->total_processed += (uint32_t)size;
    ctx->header.checksum ^= (uint8_t)(size & 0xFF);
    
    return 1;
}

void print_compression_stats(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression Statistics:\n");
    printf("Magic: 0x%08X\n", ctx->header.magic);
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100);
    printf("Total processed: %u bytes\n", ctx->total_processed);
    printf("Algorithm: %u\n", ctx->header.algorithm);
    printf("Checksum: 0x%02X\n", ctx->header.checksum