//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: compression_stub
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    struct CompressionBlock *next;
};

struct CompressionContext {
    struct CompressionHeader header;
    struct CompressionBlock *first_block;
    struct CompressionBlock *last_block;
    uint32_t total_blocks;
    uint32_t max_block_size;
};

struct CompressionStats {
    uint32_t input_bytes;
    uint32_t output_bytes;
    uint32_t blocks_processed;
    double compression_ratio;
};

void initialize_compression_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->max_block_size = 4096;
}

void cleanup_compression_context(struct CompressionContext *ctx) {
    struct CompressionBlock *current = ctx->first_block;
    while (current != NULL) {
        struct CompressionBlock *next = current->next;
        if (current->data != NULL) {
            free(current->data);
        }
        free(current);
        current = next;
    }
    ctx->first_block = NULL;
    ctx->last_block = NULL;
}

int add_compression_block(struct CompressionContext *ctx, const uint8_t *data, uint32_t size) {
    if (ctx == NULL || data == NULL || size == 0 || size > ctx->max_block_size) {
        return 0;
    }
    
    struct CompressionBlock *new_block = malloc(sizeof(struct CompressionBlock));
    if (new_block == NULL) {
        return 0;
    }
    
    new_block->data = malloc(size);
    if (new_block->data == NULL) {
        free(new_block);
        return 0;
    }
    
    memcpy(new_block->data, data, size);
    new_block->block_size = size;
    new_block->offset = ctx->header.original_size;
    new_block->next = NULL;
    
    ctx->header.original_size += size;
    ctx->total_blocks++;
    
    if (ctx->last_block == NULL) {
        ctx->first_block = new_block;
        ctx->last_block = new_block;
    } else {
        ctx->last_block->next = new_block;
        ctx->last_block = new_block;
    }
    
    return 1;
}

void calculate_compression_stats(const struct CompressionContext *ctx, struct CompressionStats *stats) {
    if (ctx == NULL || stats == NULL) {
        return;
    }
    
    memset(stats, 0, sizeof(struct CompressionStats));
    stats->input_bytes = ctx->header.original_size;
    stats->output_bytes = ctx->header.compressed_size;
    stats->blocks_processed = ctx->total_blocks;
    
    if (stats->input_bytes > 0) {
        stats->compression_ratio = (double)stats->output_bytes / stats->input_bytes;
    } else {
        stats->compression_ratio = 0.0;
    }
}

void simulate_compression(struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    struct CompressionBlock *current = ctx->first_block;
    uint32_t total_compressed = 0;
    
    while (current != NULL) {
        uint32_t compressed_size = current->block_size - (current->block_size / 10);
        if (compressed_size < 1) {
            compressed_size = 1;
        }
        total_compressed += compressed_size;
        current = current->next;
    }
    
    ctx->header.compressed_size = total_compressed;
    ctx->header.algorithm = 1;
    ctx->header.checksum = 0xAB;
}

void print_compression_report(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    struct CompressionStats stats;
    calculate_compression_stats(ctx, &stats);
    
    printf("Compression Report:\n");
    printf("  Input size: %u bytes\n", stats.input_bytes);
    printf("  Output size: %u bytes\n", stats.output_bytes);
    printf("  Blocks processed: %u\n", stats.blocks_processed);
    printf("  Compression ratio: %.2f\n", stats.compression_ratio);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
    printf("  Checksum: 0x%02X\n", ctx->header.checksum);
}

int main(void) {
    struct CompressionContext ctx;
    initialize_compression_context(&ctx);
    
    uint8_t sample_data1[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};
    uint8_t sample_data2[] = {0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74};
    uint8_t sample_data