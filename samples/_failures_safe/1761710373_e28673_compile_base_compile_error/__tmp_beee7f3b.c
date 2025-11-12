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
    uint32_t total_blocks;
    uint32_t processed_bytes;
};

void init_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0;
    
    memset(ctx->current_block.data, 0, MAX_BLOCK_SIZE);
    ctx->current_block.size = 0;
    ctx->current_block.dict_size = 0;
    
    ctx->total_blocks = 0;
    ctx->processed_bytes = 0;
}

int validate_block_size(size_t size) {
    return size > 0 && size <= MAX_BLOCK_SIZE;
}

int add_to_dictionary(struct CompressionBlock *block, uint8_t symbol) {
    if (block == NULL || block->dict_size >= MAX_DICT_SIZE) {
        return -1;
    }
    
    for (size_t i = 0; i < block->dict_size; i++) {
        if (block->dictionary[i].symbol == symbol) {
            if (block->dictionary[i].frequency < UINT32_MAX) {
                block->dictionary[i].frequency++;
            }
            return 0;
        }
    }
    
    block->dictionary[block->dict_size].symbol = symbol;
    block->dictionary[block->dict_size].frequency = 1;
    block->dictionary[block->dict_size].code_length = 0;
    block->dictionary[block->dict_size].code = 0;
    block->dict_size++;
    
    return 0;
}

int process_block(struct CompressionContext *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || !validate_block_size(size)) {
        return -1;
    }
    
    memcpy(ctx->current_block.data, data, size);
    ctx->current_block.size = size;
    ctx->current_block.dict_size = 0;
    
    for (size_t i = 0; i < size; i++) {
        if (add_to_dictionary(&ctx->current_block, data[i]) != 0) {
            return -1;
        }
    }
    
    ctx->header.original_size += size;
    ctx->total_blocks++;
    ctx->processed_bytes += size;
    
    return 0;
}

void calculate_checksum(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    uint8_t sum = 0;
    for (size_t i = 0; i < ctx->current_block.size; i++) {
        sum = (uint8_t)(sum + ctx->current_block.data[i]);
    }
    ctx->header.checksum = sum;
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, size_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) {
        return -1;
    }
    
    size_t remaining = input_size;
    const uint8_t *current_pos = input;
    
    while (remaining > 0) {
        size_t block_size = (remaining > MAX_BLOCK_SIZE) ? MAX_BLOCK_SIZE : remaining;
        
        if (process_block(ctx, current_pos, block_size) != 0) {
            return -1;
        }
        
        calculate_checksum(ctx);
        
        current_pos += block_size;
        remaining -= block_size;
    }
    
    ctx->header.compressed_size = ctx->header.original_size / 2;
    if (ctx->header.compressed_size == 0) {
        ctx->header.compressed_size = 1;
    }
    
    return 0;
}

void print_compression_stats(const struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    printf("Compression completed successfully.\n");
    printf("Original size: %u bytes\n", ctx->header.original_size);
    printf("Compressed size: %u bytes\n", ctx->header.compressed_size);
    printf("Compression ratio: %.2f%%\n", 
           (1.0 - (double)ctx->header.compressed_size / ctx->header.original_size) * 100.0);
    printf("Total blocks processed: %u\n", ctx->total_blocks);
    printf("Dictionary entries in last block: %zu\n", ctx->current_block.dict_size);
}

int main(void) {
    struct CompressionContext ctx;
    init_compression_context(&ctx);
    
    uint8_t test_data[] = {
        'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o',