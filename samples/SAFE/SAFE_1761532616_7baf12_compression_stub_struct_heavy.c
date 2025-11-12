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

void initialize_context(struct CompressionContext *ctx) {
    memset(ctx, 0, sizeof(struct CompressionContext));
    ctx->header.magic = 0x43535455;
    ctx->max_block_size = 4096;
}

void cleanup_context(struct CompressionContext *ctx) {
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
    new_block->offset = ctx->header.compressed_size;
    new_block->next = NULL;
    
    if (ctx->last_block == NULL) {
        ctx->first_block = new_block;
        ctx->last_block = new_block;
    } else {
        ctx->last_block->next = new_block;
        ctx->last_block = new_block;
    }
    
    ctx->header.compressed_size += size;
    ctx->total_blocks++;
    return 1;
}

void calculate_stats(const struct CompressionContext *ctx, struct CompressionStats *stats) {
    if (ctx == NULL || stats == NULL) {
        return;
    }
    
    stats->input_bytes = ctx->header.original_size;
    stats->output_bytes = ctx->header.compressed_size;
    stats->blocks_processed = ctx->total_blocks;
    
    if (ctx->header.original_size > 0) {
        stats->compression_ratio = (double)ctx->header.compressed_size / ctx->header.original_size;
    } else {
        stats->compression_ratio = 0.0;
    }
}

int compress_data(struct CompressionContext *ctx, const uint8_t *input, uint32_t input_size) {
    if (ctx == NULL || input == NULL || input_size == 0) {
        return 0;
    }
    
    ctx->header.original_size = input_size;
    ctx->header.compressed_size = 0;
    
    uint32_t processed = 0;
    while (processed < input_size) {
        uint32_t remaining = input_size - processed;
        uint32_t block_size = (remaining < ctx->max_block_size) ? remaining : ctx->max_block_size;
        
        if (!add_compression_block(ctx, input + processed, block_size)) {
            return 0;
        }
        
        processed += block_size;
    }
    
    return 1;
}

void print_compression_info(const struct CompressionContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    struct CompressionStats stats;
    calculate_stats(ctx, &stats);
    
    printf("Compression Results:\n");
    printf("  Original size: %u bytes\n", stats.input_bytes);
    printf("  Compressed size: %u bytes\n", stats.output_bytes);
    printf("  Blocks processed: %u\n", stats.blocks_processed);
    printf("  Compression ratio: %.2f\n", stats.compression_ratio);
    printf("  Algorithm: %u\n", ctx->header.algorithm);
}

int main() {
    struct CompressionContext ctx;
    initialize_context(&ctx);
    
    ctx.header.algorithm = 1;
    
    uint8_t test_data[8192];
    for (int i = 0; i < 8192; i++) {
        test_data[i] = (uint8_t)(i % 256);
    }
    
    printf("Starting compression test...\n");
    
    if (!compress_data(&ctx, test_data, sizeof(test_data))) {
        printf("Compression failed!\n");
        cleanup_context(&ctx);
        return 1;
    }
    
    print_compression_info(&ctx);
    cleanup_context(&ctx);
    
    return 0;
}