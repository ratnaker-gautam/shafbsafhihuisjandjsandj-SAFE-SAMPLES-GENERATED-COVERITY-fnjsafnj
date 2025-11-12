//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm_id;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_offset;
    uint16_t block_size;
    uint8_t compression_level;
    uint8_t reserved;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
    uint8_t *input_data;
    uint32_t input_size;
};

int validate_header(const struct CompressionHeader *header) {
    if (header == NULL) return 0;
    if (header->magic != 0x43535442) return 0;
    if (header->original_size > 104857600) return 0;
    if (header->compressed_size > 104857600) return 0;
    if (header->algorithm_id > 15) return 0;
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, uint32_t offset, uint16_t size, uint8_t level) {
    if (ctx == NULL) return 0;
    if (offset > ctx->input_size) return 0;
    if (size == 0 || size > 65535) return 0;
    if (level > 9) return 0;
    
    if (ctx->block_count >= ctx->max_blocks) {
        uint32_t new_max = ctx->max_blocks * 2;
        if (new_max < ctx->max_blocks) return 0;
        struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_max * sizeof(struct CompressionBlock));
        if (new_blocks == NULL) return 0;
        ctx->blocks = new_blocks;
        ctx->max_blocks = new_max;
    }
    
    ctx->blocks[ctx->block_count].block_offset = offset;
    ctx->blocks[ctx->block_count].block_size = size;
    ctx->blocks[ctx->block_count].compression_level = level;
    ctx->blocks[ctx->block_count].reserved = 0;
    ctx->block_count++;
    
    return 1;
}

uint8_t calculate_checksum(const uint8_t *data, uint32_t size) {
    if (data == NULL || size == 0) return 0;
    uint8_t checksum = 0;
    for (uint32_t i = 0; i < size; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int compress_data(struct CompressionContext *ctx) {
    if (ctx == NULL || ctx->input_data == NULL) return 0;
    if (ctx->input_size == 0 || ctx->input_size > 104857600) return 0;
    
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = ctx->input_size;
    ctx->header.algorithm_id = 1;
    ctx->header.flags = 0;
    
    uint32_t processed = 0;
    uint8_t compression_level = 6;
    
    while (processed < ctx->input_size) {
        uint16_t block_size = 4096;
        if (ctx->input_size - processed < block_size) {
            block_size = ctx->input_size - processed;
        }
        
        if (!add_compression_block(ctx, processed, block_size, compression_level)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    ctx->header.compressed_size = processed;
    ctx->header.checksum = calculate_checksum(ctx->input_data, ctx->input_size);
    
    return 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
        ctx->blocks = NULL;
    }
    if (ctx->input_data != NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
    }
    ctx->block_count = 0;
    ctx->max_blocks = 0;
    ctx->input_size = 0;
}

int initialize_compression_context(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0) return 0;
    if (size > 104857600) return 0;
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    
    ctx->input_data = malloc(size);
    if (ctx->input_data == NULL) return 0;
    memcpy(ctx->input_data, data, size);
    ctx->input_size = size;
    
    ctx->max_blocks = 16;
    ctx->blocks = malloc(ctx->max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        free(ctx->input_data);
        ctx->input_data = NULL;
        return 0;
    }
    
    return 1;
}

int main(void) {
    struct CompressionContext ctx;
    uint8_t test_data[8192];
    
    for (int i = 0; i < 8192; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    if (!initialize_compression_context(&ctx, test_data,