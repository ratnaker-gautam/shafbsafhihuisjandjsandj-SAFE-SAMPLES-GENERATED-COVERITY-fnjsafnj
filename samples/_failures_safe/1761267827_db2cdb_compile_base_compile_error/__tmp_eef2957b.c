//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t checksum;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t offset;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

struct CompressionStats {
    uint32_t total_input;
    uint32_t total_output;
    double compression_ratio;
    uint32_t blocks_processed;
};

int init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0 || max_blocks > 1000) {
        return 0;
    }
    
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        return 0;
    }
    
    for (uint32_t i = 0; i < max_blocks; i++) {
        ctx->blocks[i].data = NULL;
        ctx->blocks[i].block_size = 0;
        ctx->blocks[i].offset = 0;
    }
    
    ctx->max_blocks = max_blocks;
    ctx->header.magic = 0x434D5052;
    ctx->header.algorithm = 1;
    return 1;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data != NULL) {
            free(ctx->blocks[i].data);
        }
    }
    
    if (ctx->blocks != NULL) {
        free(ctx->blocks);
    }
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > 65536) {
        return 0;
    }
    
    if (ctx->block_count >= ctx->max_blocks) {
        return 0;
    }
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->data = malloc(size);
    if (block->data == NULL) {
        return 0;
    }
    
    memcpy(block->data, data, size);
    block->block_size = size;
    block->offset = ctx->header.original_size;
    
    ctx->header.original_size += size;
    ctx->header.compressed_size += (size > 10 ? size - 2 : size);
    ctx->block_count++;
    
    return 1;
}

void calculate_stats(const struct CompressionContext *ctx, struct CompressionStats *stats) {
    if (ctx == NULL || stats == NULL) return;
    
    memset(stats, 0, sizeof(struct CompressionStats));
    stats->total_input = ctx->header.original_size;
    stats->total_output = ctx->header.compressed_size;
    stats->blocks_processed = ctx->block_count;
    
    if (stats->total_input > 0) {
        stats->compression_ratio = (double)stats->total_output / stats->total_input;
    } else {
        stats->compression_ratio = 0.0;
    }
}

int validate_compression_data(const struct CompressionContext *ctx) {
    if (ctx == NULL) return 0;
    
    if (ctx->header.magic != 0x434D5052) {
        return 0;
    }
    
    if (ctx->block_count > ctx->max_blocks) {
        return 0;
    }
    
    uint32_t total_size = 0;
    for (uint32_t i = 0; i < ctx->block_count; i++) {
        if (ctx->blocks[i].data == NULL) {
            return 0;
        }
        if (ctx->blocks[i].block_size > 65536) {
            return 0;
        }
        total_size += ctx->blocks[i].block_size;
    }
    
    return total_size == ctx->header.original_size;
}

int main(void) {
    struct CompressionContext ctx;
    struct CompressionStats stats;
    
    if (!init_compression_context(&ctx, 5)) {
        fprintf(stderr, "Failed to initialize compression context\n");
        return 1;
    }
    
    uint8_t test_data1[] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41};
    uint8_t test_data2[] = {0x42, 0x43, 0x44, 0x45, 0x46};
    uint8_t test_data3[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    
    if (!add_compression_block(&ctx, test_data1, sizeof(test_data1))) {
        fprintf(stderr, "Failed to add block 1\n");
        cleanup_compression_context(&ctx);
        return 1;
    }