//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t original_size;
    uint32_t compressed_size;
    uint16_t algorithm;
    uint8_t flags;
    uint8_t reserved;
};

struct CompressionBlock {
    uint32_t block_size;
    uint32_t checksum;
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

void init_compression_context(struct CompressionContext *ctx, uint32_t max_blocks) {
    if (ctx == NULL || max_blocks == 0) return;
    
    ctx->header.magic = 0x43535442;
    ctx->header.original_size = 0;
    ctx->header.compressed_size = 0;
    ctx->header.algorithm = 1;
    ctx->header.flags = 0;
    ctx->header.reserved = 0;
    
    ctx->blocks = malloc(max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) return;
    
    for (uint32_t i = 0; i < max_blocks; i++) {
        ctx->blocks[i].block_size = 0;
        ctx->blocks[i].checksum = 0;
        ctx->blocks[i].data = NULL;
    }
    
    ctx->block_count = 0;
    ctx->max_blocks = max_blocks;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->blocks != NULL) {
        for (uint32_t i = 0; i < ctx->block_count; i++) {
            if (ctx->blocks[i].data != NULL) {
                free(ctx->blocks[i].data);
            }
        }
        free(ctx->blocks);
    }
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || ctx->block_count >= ctx->max_blocks) {
        return 0;
    }
    
    if (size > 65536) return 0;
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->data = malloc(size);
    if (block->data == NULL) return 0;
    
    memcpy(block->data, data, size);
    block->block_size = size;
    
    uint32_t checksum = 0;
    for (uint32_t i = 0; i < size; i++) {
        checksum = (checksum << 3) ^ data[i];
    }
    block->checksum = checksum;
    
    ctx->header.original_size += size;
    ctx->header.compressed_size += (size > 128) ? size - 32 : size;
    ctx->block_count++;
    
    return 1;
}

void calculate_stats(const struct CompressionContext *ctx, struct CompressionStats *stats) {
    if (ctx == NULL || stats == NULL) return;
    
    stats->total_input = ctx->header.original_size;
    stats->total_output = ctx->header.compressed_size;
    stats->blocks_processed = ctx->block_count;
    
    if (ctx->header.original_size > 0) {
        stats->compression_ratio = (double)ctx->header.compressed_size / ctx->header.original_size;
    } else {
        stats->compression_ratio = 0.0;
    }
}

void print_compression_stats(const struct CompressionStats *stats) {
    if (stats == NULL) return;
    
    printf("Compression Statistics:\n");
    printf("Total input size: %u bytes\n", stats->total_input);
    printf("Total output size: %u bytes\n", stats->total_output);
    printf("Blocks processed: %u\n", stats->blocks_processed);
    printf("Compression ratio: %.2f\n", stats->compression_ratio);
}

int main(void) {
    struct CompressionContext ctx;
    struct CompressionStats stats;
    
    init_compression_context(&ctx, 10);
    
    uint8_t test_data1[] = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48};
    uint8_t test_data2[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30};
    uint8_t test_data3[256];
    for (int i = 0; i < 256; i++) {
        test_data3[i] = (uint8_t)i;
    }
    
    if (!add_compression_block(&ctx, test_data1, sizeof(test_data1))) {
        printf("Failed to add block 1\n");
        cleanup_compression_context(&ctx);
        return 1;
    }
    
    if (!add_compression_block(&ctx, test_data2, sizeof(test_data2))) {
        printf("Failed to add block 2\n");
        cleanup_compression_context(&ctx);
        return 1;
    }