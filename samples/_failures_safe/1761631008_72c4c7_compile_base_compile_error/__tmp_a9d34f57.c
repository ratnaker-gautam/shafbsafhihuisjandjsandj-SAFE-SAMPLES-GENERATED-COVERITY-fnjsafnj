//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

struct CompressionHeader {
    uint32_t magic;
    uint32_t version;
    uint64_t original_size;
    uint64_t compressed_size;
    uint32_t checksum;
    uint8_t algorithm;
    uint8_t flags;
};

struct CompressionBlock {
    uint32_t block_id;
    uint32_t offset;
    uint32_t size;
    uint32_t compressed_size;
    uint8_t *data;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *blocks;
    uint32_t block_count;
    uint32_t max_blocks;
};

struct CompressionStats {
    uint64_t total_input;
    uint64_t total_output;
    uint32_t blocks_processed;
    double compression_ratio;
};

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->header.magic = 0x43535455;
    ctx->header.version = 1;
    ctx->max_blocks = 16;
    ctx->blocks = malloc(ctx->max_blocks * sizeof(struct CompressionBlock));
    if (ctx->blocks == NULL) {
        exit(EXIT_FAILURE);
    }
    memset(ctx->blocks, 0, ctx->max_blocks * sizeof(struct CompressionBlock));
}

void cleanup_context(struct CompressionContext *ctx) {
    if (ctx->blocks != NULL) {
        for (uint32_t i = 0; i < ctx->block_count; i++) {
            if (ctx->blocks[i].data != NULL) {
                free(ctx->blocks[i].data);
            }
        }
        free(ctx->blocks);
    }
}

int validate_header(const struct CompressionHeader *header) {
    if (header->magic != 0x43535455) {
        return 0;
    }
    if (header->version != 1) {
        return 0;
    }
    if (header->algorithm > 3) {
        return 0;
    }
    return 1;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > UINT32_MAX / 2) {
        return 0;
    }
    
    if (ctx->block_count >= ctx->max_blocks) {
        uint32_t new_max = ctx->max_blocks * 2;
        if (new_max <= ctx->max_blocks) {
            return 0;
        }
        struct CompressionBlock *new_blocks = realloc(ctx->blocks, new_max * sizeof(struct CompressionBlock));
        if (new_blocks == NULL) {
            return 0;
        }
        ctx->blocks = new_blocks;
        ctx->max_blocks = new_max;
    }
    
    struct CompressionBlock *block = &ctx->blocks[ctx->block_count];
    block->block_id = ctx->block_count;
    block->offset = 0;
    block->size = size;
    block->compressed_size = size;
    
    block->data = malloc(size);
    if (block->data == NULL) {
        return 0;
    }
    
    memcpy(block->data, data, size);
    ctx->block_count++;
    
    ctx->header.original_size += size;
    ctx->header.compressed_size += size;
    
    return 1;
}

void calculate_stats(const struct CompressionContext *ctx, struct CompressionStats *stats) {
    memset(stats, 0, sizeof(*stats));
    if (ctx == NULL || ctx->block_count == 0) {
        return;
    }
    
    stats->total_input = ctx->header.original_size;
    stats->total_output = ctx->header.compressed_size;
    stats->blocks_processed = ctx->block_count;
    
    if (stats->total_input > 0) {
        stats->compression_ratio = (double)stats->total_output / (double)stats->total_input;
    } else {
        stats->compression_ratio = 1.0;
    }
}

void print_compression_stats(const struct CompressionStats *stats) {
    printf("Compression Statistics:\n");
    printf("Total input size: %lu bytes\n", stats->total_input);
    printf("Total output size: %lu bytes\n", stats->total_output);
    printf("Blocks processed: %u\n", stats->blocks_processed);
    printf("Compression ratio: %.2f\n", stats->compression_ratio);
}

int main(void) {
    struct CompressionContext ctx;
    struct CompressionStats stats;
    
    initialize_context(&ctx);
    
    uint8_t test_data1[] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41};
    uint8_t test_data2[] = {0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A};
    uint8_t test_data3[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    
    if (!add_compression_block(&ctx, test_data1, sizeof(test_data1))) {
        cleanup_context(&ctx);
        return EX